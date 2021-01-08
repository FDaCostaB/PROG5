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
    uint32_t instruction;
    if(arm_fetch(p, &instruction)){
        exit(EXIT_FAILURE).
    }
    else{
        switch(get_bits(instruction, 27, 25))
        {
            case 0b000:
                /* miscellaneous*/
                break;
            case 0b001:
                /* Move immediate to status register */
                /* Undefined instruction  */
                /* Data processing immediate */
                break;
            case 0b010:
                arm_load_store(p, instruction);
                break;
            case 0b011:
                /* Media instructions */
                /* Load/store register offset */
                break;
            case 0b100:
                arm_load_store_multiple(p, instruction);
                break;
            case 0b101:
                arm_branch(p, instruction);
                break;
            case 0b110:
                arm_coprocessor_load_store(p, instruction);
                break;
            case 0b111:
                arm_coprocessor_others_swi(p, instruction);
                break;
            default:
                break;
        }
    }
    return 0;
}

static int arm_execute_instruction(arm_core p) {
    uint32_t ins;
    arm_fetch(p,&ins);
    printf("%08x \n",ins);
    uint8_t champ = (uint8_t)get_bits(instruction, 27, 25);
    switch (champ) {
        case 0:
        case 1:
            /* Move immediate to status register */
            /* Undefined instruction  */
            /* Data processing immediate */
            arm_data_processing_shift(p, ins);
            break;
        case 2:
        case 3:
            /* Media instructions */
            /* Load/store register offset */
            arm_load_store(p, instruction);
            break;
        case 4:

            break;
        case 5:
            arm_branch(p, ins);
            break;
        case 6:

            break;
        case 7:

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