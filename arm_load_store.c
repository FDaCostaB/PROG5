/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"

int arm_load_store(arm_core p, uint32_t ins){
    // A5.2.1 Encoding ( Addressing Mode 2 - Load and Store Word or Unsigned Byte )
    uint32_t rn = get_bits(ins,19,16);
    uint32_t address = 0;
    uint32_t rd = get_bits(ins,15,12);
    uint32_t operateur = get_bit(ins, 20); //OP = LDR ou STR (Bit L)
    uint32_t I = get_bit(ins, 25);
    uint32_t P = get_bit(ins, 24);
    uint32_t U = get_bit(ins, 23);
    uint32_t B = get_bit(ins, 22);
    uint32_t W = get_bit(ins, 21);
    uint32_t index = 0;

    //Addressing Mode
    if (I==0){
        if(P==1 || ( P==0 && ConditionPassed(arm_read_cpsr(p),ins) ) ){
            index = get_bits(ins,11,0); //offset_12
        }
    } else {
        // A5.2.4 Load and Store Word or Unsigned Byte - Scaled register offset
        if(P==1 || ( P==0 && ConditionPassed(arm_read_cpsr(p),ins) )){
            uint16_t shift = get_bits(ins,6,5);
            uint16_t shift_imm = get_bits(ins,11,7);
            uint16_t rm = get_bits(ins,3,0);
            switch(shift) {
                case 0b00 : /* LSL */
                    index = arm_read_register(p, rm) << shift_imm;
                    break;
                case 0b01 :/* LSR */
                    if (shift_imm == 0) {
                        index = 00 ; //arm_read_register(p, rm);
                    } else { /* LSR #32 */
                        index = arm_read_register(p, rm) >> shift_imm;
                    }
                case 0b10 :/* ASR */
                    if (shift_imm == 0) { /* ASR #32 */
                        if (get_bit(arm_read_register(p,rm),31) == 1)
                            index = 0xFFFFFFFF;
                        else
                            index = 0;
                    } else {
                        index = asr(arm_read_register(p, rm),shift_imm);
                    }
                case 0b11 : /* ROR or RRX */
                    if (shift_imm == 0) {/* RRX */
                        index = (get_bit(arm_read_cpsr(p),C) << 31) || (arm_read_register(p, rm) >> 1);
                    } else {
                        index = ror(arm_read_register(p, rm),shift_imm);
                    }/* ROR */
            }
        }
    }
    //A5.2.2 Load and Store Word or Unsigned Byte - Immediate offset
    // P == 0 Indicates the use of post-indexed addressing.
    // P == 1 Indicates the use of offset addressing or pre-indexed addressing.
    if(U == 1) {
        if(P==1) address = arm_read_register(p,rn) + index  ;
        else address = arm_read_register(p, rn);
    }
    else{ /* U == 0 */
        if(P==1) address = arm_read_register(p,rn) - index  ;
        else address = arm_read_register(p, rn);
    }
    if(P==0 && ConditionPassed(arm_read_cpsr(p),ins)) {
        if(W==1)arm_write_usr_register(p, rn, U?arm_read_register(p,rn)+index:arm_read_register(p,rn)-index);
        else arm_write_register(p, rn,U?arm_read_register(p,rn)+index:arm_read_register(p,rn)-index);
    }
    if(P==1 && ConditionPassed(arm_read_cpsr(p),ins) && W==1 ) arm_write_usr_register(p, rn, address);
    // On execute le LDR
    if(operateur == 1){
        // A4.1.23 LDR
        if (ConditionPassed(arm_read_cpsr(p),ins)){
            uint32_t value = 0;
            // A4.1.23 LDR +  A4.1.24 LDRB
            if (B==1){
                arm_read_byte(p, address, (uint8_t *)&value);
            } else {
                arm_read_word(p,address,&value);
            }
            if (rd ==  15){
                arm_write_register(p,15,value & 0xFFFFFFFE);
                arm_write_cpsr(p,get_bit(value,0)?set_bit(arm_read_cpsr(p),T):clr_bit(arm_read_cpsr(p),T));
            }
            else{
                arm_write_register(p,rd,value);
            }
        }
    } else { // On execute le STR
        // A4.1.?? STR
        if (ConditionPassed(arm_read_cpsr(p),ins)){
            if (B==1){
                arm_write_byte(p, address, (uint8_t)get_bits(arm_read_register(p,rd),7,0));
            } else {
                arm_write_word(p, address, arm_read_register(p,rd));
            }
        }
    }
    return 0;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    uint32_t rn = get_bits(ins,19,16);
    uint32_t P = get_bit(ins, 24);
    uint32_t U = get_bit(ins, 23);
    uint32_t W = get_bit(ins, 21);
    uint32_t operateur = get_bit(ins, 20);
    uint8_t nb_reg = 0;
    uint32_t start_address = 0,end_address = 0, address = 0, value =0;
    for(int i = 0;i<=14;i++){
        nb_reg+=get_bit(ins, i);
    }
    if(P==0){
            if(U==1){
                start_address = arm_read_register(p,rn);
                end_address = (arm_read_register(p,rn) + nb_reg * 4) - 4;
                if (ConditionPassed(arm_read_cpsr(p),ins) && W == 1)
                        arm_write_register(p,rn,arm_read_register(p,rn) + nb_reg * 4);
            } else { // U == 0
                start_address = arm_read_register(p,rn) - (nb_reg * 4) + 4;
                end_address = arm_read_register(p,rn);
                if (ConditionPassed(arm_read_cpsr(p),ins) && W == 1)
                        arm_write_register(p,rn,arm_read_register(p,rn) - (nb_reg * 4) );
            }
        } else { // P == 1
            if(U==1){
                start_address = arm_read_register(p,rn) + 4;
                end_address = arm_read_register(p,rn) + (nb_reg * 4);
                if (ConditionPassed(arm_read_cpsr(p),ins) && W == 1)
                        arm_write_register(p,rn,arm_read_register(p,rn) + (nb_reg * 4) );
            } else { // U == 0
                start_address = arm_read_register(p,rn) - (nb_reg * 4);
                end_address = arm_read_register(p,rn) - 4;
                if (ConditionPassed(arm_read_cpsr(p),ins) && W == 1)
                        arm_write_register(p,rn,arm_read_register(p,rn) - (nb_reg * 4));
            }
        }
    //A4.1.20 LDM (1)
    if(operateur == 1){
        if(ConditionPassed(arm_read_cpsr(p),ins)){
            address = start_address;
            for(int ri = 0;ri<=14;ri++){
                if (get_bit(ins, ri) == 1){
                    arm_read_word(p,address,&value);
                    arm_write_register(p,ri,value);
                    address = address + 4;
                }
            }
            if (get_bit(ins, 15) == 1){
                arm_read_word(p,address,&value);
                arm_write_register(p,15,value && 0xFFFFFFFE);
                arm_write_cpsr(p, get_bit(value,0) ? set_bit(arm_read_cpsr(p),T) : clr_bit(arm_read_cpsr(p),T) );
            }
            address = address + 4;
        }
    } else {//A4.1.?? STM (1)
        if(ConditionPassed(arm_read_cpsr(p),ins)){
            address = start_address;
            for(int ri = 0;ri<=14;ri++){
                if (get_bit(ins,ri) == 1){
                    arm_write_word(p,address,arm_read_register(p,ri));        
                    address = address + 4 ;
                }
        }
    }       
}
    if(end_address != address - 4) return 1;
    return 0;
}

int arm_load_store_half(arm_core p, uint32_t ins) {
    uint32_t rn = get_bits(ins,19,16);
    uint32_t rd = get_bits(ins,15,12);
    uint32_t rm_immedL = get_bits(ins,3,0);
    uint32_t immedH = get_bits(ins,11,8);
    uint32_t P = get_bit(ins, 24);
    uint32_t U = get_bit(ins, 23);
    uint32_t I = get_bit(ins, 22);
    uint32_t W = get_bit(ins, 21);
    uint32_t operateur = get_bit(ins, 20);
    uint8_t offset_8 = 0;
    uint32_t address = 0, value = 0;

    if(get_bits(ins,7,4) != 0b1011) return 1;
    if(I==1) offset_8 = (immedH << 4) | rm_immedL ;
    else offset_8 = arm_read_register(p,rm_immedL);

    if(P==1){
        address = U? arm_read_register(p,rn) + offset_8 : arm_read_register(p,rn) - offset_8 ;
    } else {
        address = arm_read_register(p,rn);
        arm_write_register(p,rn,arm_read_register(p,rn)+rm_immedL);
    }

    if(W==1&&P==0) printf("UNPREDICTABLE");
    if(W==1&& ConditionPassed(arm_read_cpsr(p),ins)) arm_write_register(p,rn,address);

    //A4.1.29 LDRH
    if(operateur == 1){

        if (ConditionPassed(arm_read_cpsr(p),ins) ){
            arm_read_half(p,address,(uint16_t *)&value);
            arm_write_register(p,rd,value);
        }
    } else { //A4.1.104 STRH
        if(ConditionPassed(arm_read_cpsr(p),ins)){                                
            arm_write_half(p,address,(uint16_t)get_bits(arm_read_register(p,rd),15,0));
        }
    }
    return 0;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}
