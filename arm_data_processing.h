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
#ifndef __ARM_DATA_PROCESSING_H__
#define __ARM_DATA_PROCESSING_H__
#include <stdint.h>
#include "arm_core.h"


void updateZN(arm_core p,uint32_t resultat);
uint32_t arm_and (uint32_t val1, uint32_t val2);
uint32_t arm_eor (uint32_t val1, uint32_t val2);
uint32_t arm_sub (uint32_t val1, uint32_t val2);
uint32_t arm_orr(uint32_t val1, uint32_t val2);
uint32_t arm_add(uint32_t val1, uint32_t val2);
uint32_t arm_bic(uint32_t val1, uint32_t val2);
void arm_mov(arm_core p,uint8_t rd,uint32_t val,int s);
uint32_t arm_sbc(arm_core p,uint32_t val1, uint32_t val2);
uint32_t arm_adc(arm_core p,uint32_t val1,uint32_t val2);
void arm_mvn(arm_core p,uint8_t rd, uint32_t val,int s);
void arm_teq(arm_core p,uint32_t val1,uint32_t val2);
void arm_tst(arm_core p,uint32_t val1,uint32_t val2);
void arm_cmp(arm_core p,uint32_t val1,uint32_t val2);
void arm_cmn(arm_core p,uint32_t  val1,uint32_t val2);
void arm_rsb(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh,  int s);
void arm_rsc(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh,  int s);

int arm_data_processing_shift(arm_core p, uint32_t ins);
int arm_data_processing_immediate_msr(arm_core p, uint32_t ins);

#endif
