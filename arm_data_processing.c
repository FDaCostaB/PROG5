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
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

uint32_t arm_and (uint32_t val1, uint32_t val2);

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
	uint8_t opcode = (uint8_t)((ins & 0xf00000) >> 20);
	switch (opcode){
		case 0:
			apply(p,ins,arm_and);
			break;
		case 1:
			arm_eor(p,ins);
			break;
		case 2:
			arm_sub(p,ins);
			break;
		case 3:
			arm_rsb(p,ins);
			break;
		case 4:
			arm_add(p,ins);
			break;
		case 5:
			arm_adc(p,ins);
			break;
		case 6:
			arm_sbc(p,ins);
			break;
		case 7:
			arm_rsc(p,ins);
			break;
		case 8:
			arm_tst(p,ins);
			break;
		case 9:
			arm_teq(p,ins);
			break;
		case 10:
			arm_cmp(p,ins);
			break;
		case 11:
			arm_cmn(p,ins);
			break;
		case 12:
			arm_orr(p,ins);
			break;
		case 13:
			arm_mov(p,ins);
			break;
		case 14:
			arm_bic(p,ins);
			break;
		case 15:
			arm_mvn(p,ins);
			break;
	}
    return UNDEFINED_INSTRUCTION;
}

int shifter_operand(arm_core p, uint32_t ins,uint32_t (*fonction)(uint32_t val1,uint32_t val2) ){
	uint32_t rn = (ins << (sizeof(ins) - 19)) >> 28; //adresse registre source
	uint32_t rd = (ins << (sizeof(ins) - 15)) >> 28; //adresse registre destination
	uint32_t operateur = (ins << sizeof(ins) -20) >> 20; //I
	uint32_t val1;
	uint32_t val2;
	uint32_t res;

	if (operateur==1){ //32-bit immediate -- a modifier pour valeurs interdites 
		uint8_t rotation = (uint8_t)((ins & 0xf00) >> 8);
		uint8_t immed_8;
		memory_read_word(p->mem,rn,&val1);
		immed_8 = (uint8_t) ins & 0xff;
		immed_8 = (immed_8 >> rotation*2) | (immed_8 << (8-rotation*2)); //rotation de rotation_imm x 2 bits 
		val2 = (uint32_t)immed_8;
	} else {
		if (shifter_op<= 15){//shifter_operand = Rm
			uint32_t rm = (shifter_op & 0xf);
			memory_read_word(p->mem,rn,&val1);
			memory_read_word(p->mem,rm,&val2);
			res = val1+val2;
			arm_write_register(p,rd,res);
		}
		else if {

		}
	
	}
	res = fonction(val1,val2); //a modifier pour les fonctions a un seul paramètres ex MOV/MNV
	arm_write_register(p,rd,res);

}