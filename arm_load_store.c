/*
Armator - simulateur de jeu d'instruction ARMv5T à but pédagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
	 Bâtiment IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hères
*/
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"

cond arm_get_cond(uint32_t ins){
    uint8_t val = (uint8_t)((ins & 0xf0000000) >> 28);
    return (cond) val;
}

int ConditionPasse(arm_core p,uint32_t ins){
    uint32_t cpsr = arm_read_cpsr(p);
    switch (arm_get_cond(ins)) {
        case EQ:
            return get_bit(cpsr,Z);
            break;
        case NE:
            return !get_bit(cpsr,Z);
            break;
        case CS_HS:
            return get_bit(cpsr,C);
            break;
        case CC_LO:
            return !get_bit(cpsr,C);
            break;
        case MI:
            return get_bit(cpsr,N);
            break;
        case PL:
            return !get_bit(cpsr,N);
            break;
        case VS:
            return get_bit(cpsr,V);
            break;
        case VC:
            return !get_bit(cpsr,V);
            break;
        case HI:
            return get_bit(cpsr,C) && !get_bit(cpsr,Z);
            break;
        case LS:
            return !get_bit(cpsr,C) || get_bit(cpsr,Z);
            break;
        case GE:
            return get_bit(cpsr,N) == get_bit(cpsr,V);
            break;
        case LT:
            return get_bit(cpsr,N) != get_bit(cpsr,V);
            break;
        case GT:
            return !get_bit(cpsr,Z) && (get_bit(cpsr,N) == get_bit(cpsr,V));
            break;
        case LE:
            return get_bit(cpsr,Z) || (get_bit(cpsr,N) != get_bit(cpsr,V));
            break;
        case AL:
            return 1;
            break;
        default:
            return 0;
            break;
    }
}

int arm_load_store(arm_core p, uint32_t ins) {
    uint32_t rn = (ins << ( sizeof(ins)*8 - 19) )>> 28; //adresse??
    uint32_t address = 0;

    uint32_t rd = (ins << ( sizeof(ins)*8 - 15) ) >> 28;
    uint32_t operateur = get_bits(instruction, 20, 20); //OP = LDR ou STR (Bit L)
    uint32_t I = get_bits(instruction, 25, 25);
    uint32_t P = get_bits(instruction, 24, 24);
    uint32_t U = get_bits(instruction, 23, 23);
    uint32_t B = get_bits(instruction, 22, 22);
    uint32_t W = get_bits(instruction, 21, 21);
    // On execute le LDR
    if(operateur == 1){
        if (I==0){
            if(P==1 || ( P==0 && ConditionPassed(p,ins) ) ){
                if(U==1){
                    address = rn + offset_12;
                } else {
                    address = rn - offset_12;
                }
            }
        } else {
            uint32_t index = 0;
            if(P==1 || ( P==0 && ConditionPassed(p,ins) )){
                switch(shift) {
                    case 0b00 : /* LSL */
                        index = Rm Logical_Shift_Left shift_imm;
                        break;
                    case 0b01 :/* LSR */
                        if (shift_imm == 0) {
                            index = 0
                        } else { /* LSR #32 */
                            index = Rm Logical_Shift_Right shift_imm
                        }
                    case 0b10 :/* ASR */
                        if (shift_imm == 0) { /* ASR #32 */
                            if (Rm[31] == 1)
                                index = 0xFFFFFFFF
                            else
                                index = 0;
                        } else {
                            index = Rm Arithmetic_Shift_Right shift_imm
                        }
                    case 0b11 : /* ROR or RRX */
                        if (shift_imm == 0) {/* RRX */
                            index = (C Flag Logical_Shift_Left 31) OR (Rm Logical_Shift_Right 1)
                        } else {
                            index = Rm Rotate_Right shift_imm
                        }/* ROR */
                }
                if(U == 1) {
                    address = Rn + index
                }
                else{ /* U == 0 */
                    address = Rn - index
                }
            }
        }
    }
    if ConditionPassed(p,ins){
        //CP15 not implemented yet
        /*if (CP15_reg1_Ubit == 0) data = Memory[address,4] Rotate_Right (8 * address[1:0]) else  CP15_reg_Ubit == 1 */
        if (B==1){
            uint8_t value = 0;
            arm_read_byte(p, address, &value);
        } else {
            uint32_t value = 0;
            arm_read_word(p,address,&value);
        }
        if (Rd ==  15){
            arm_write_register(p,15,value & 0xFFFFFFFE);
            T Bit = data[0]
        }
        else{
            arm_write_register(p,rd,value);
        }
        if ( ConditionPassed(p,ins) && ( W==1 || P==0 ) ){
            arm_write_register(p,rn,address);// P==0 rn devrait remplacer address dans le bloc de calcul
        }
    }

    return 0;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
