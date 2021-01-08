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
        uint32_t val;
        arm_read_word(p,rn,&val);
        arm_write_register(p,rd,val);
    } else {
        if (I==0){
            if(P==1 || P==0 && ConditionPassed(cond)){
                if(U==1){
                    address = rn + offset_12;
                } else {
                    address = rn - offset_12;
                }
            }
            if (B==1)arm_write_byte(p,address,arm_read_register(p,rd)) else arm_write_word(p,address,arm_read_register(p,rd));
            if (ConditionPassed(cond) && ( W==1 || P==0 ) ) Rn = address; // P==0 rn devrait remplacer address dans le bloc de calcul
        } else {
            if(P==1 || P==0 && ConditionPassed(cond)){
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
            if (B==1)arm_write_byte(p,address,arm_read_register(p,rd)) else arm_write_word(p,address,arm_read_register(p,rd));
            if (ConditionPassed(cond) && ( W==1 || P==0 ) ) Rn = address;// P==0 rn devrait remplacer address dans le bloc de calcul
            }
        }
    if ConditionPassed(cond){
        if (CP15_reg1_Ubit == 0)
            data = Memory[address,4] Rotate_Right (8 * address[1:0])
        else /* CP15_reg_Ubit == 1 */
            data = Memory[address,4];
        if (Rd == 15){
            PC = data & 0xFFFFFFFE
            T Bit = data[0]
        }
        else{
            Rd = data
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
