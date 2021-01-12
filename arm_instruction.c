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
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"

static int arm_execute_instruction(arm_core p) {
    uint32_t ins;
    if(arm_fetch(p, &ins)) {
        return 1;
    }
    printf("%08x \n",ins);
    uint8_t champ = (uint8_t)get_bits(ins, 27, 25);
    printf("%d \n",champ);
    switch (champ) {
        case 0:
            if(get_bits(ins,7,4)==0b1011 && get_bit(ins,20)==1){
                int arm_load_store_half(arm_core p, uint32_t ins);
            }else{
                arm_data_processing_shift(p, ins);
            }
            break;
        case 1:
            arm_data_processing_shift(p, ins);
            break;
        case 2:
        case 3:
            arm_load_store(p, ins);
            break;
        case 4:
            arm_load_store_multiple(p, ins);
            break;
        case 5:
            arm_branch(p, ins);
            break;
        case 6:
            arm_coprocessor_load_store(p, ins);
            break;
        case 7:
            arm_coprocessor_others_swi(p, ins);
            break;
        default:
            return 1;
            break;
    }
    return 0;
}

int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result)
        arm_exception(p, result);
    return result;
}