/*
Armator - simulateur de jeu d'instruction ARMv5T ? but p?dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G?n?rale GNU publi?e par la Free Software
Foundation (version 2 ou bien toute autre version ult?rieure choisie par vous).

Ce programme est distribu? car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous ? la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir re?u une copie de la Licence Publique Générale GNU en m?me
temps que ce programme ; si ce n'est pas le cas, écrivez ? la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
?tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 Batiment IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hères
*/

#include "registers.h"
#include "arm_constants.h"
#include <stdlib.h>
#include <string.h>

struct registers_data {
    uint32_t data[18];
    uint8_t mode;
};

registers registers_create() {
    registers r = malloc(sizeof(struct registers_data));
    r->mode = USR;
    memset(r->data,0,sizeof(uint32_t)*18);
    if (r == NULL)
        exit(EXIT_FAILURE);
    return r;
}

void registers_destroy(registers r) {
    free(r);
    r = NULL;
}

uint8_t get_mode(registers r) {
    return r->mode;
}

int current_mode_has_spsr(registers r) {
    uint8_t tmp = (uint8_t) IRQ;
    if(r->mode != tmp)
        return 1;
    return 0;
}

int in_a_privileged_mode(registers r) {
    uint8_t tmp = (uint8_t) SYS;
    if(r->mode != tmp)
        return 1;
    return 0;
}

uint32_t read_register(registers r, uint8_t reg) {
    uint32_t value=0;
    if(reg < 16)
        value=r->data[reg];
    return value;
}

uint32_t read_usr_register(registers r, uint8_t reg) {
    uint32_t value = 0;
    if(reg < 16 && r->mode == USR)
        value=r->data[reg];
    return value;
}

uint32_t read_cpsr(registers r) {
    uint32_t value=r->data[16];
    return value;
}

uint32_t read_spsr(registers r) {
    uint32_t value=r->data[17];
    return value;
}

void write_register(registers r, uint8_t reg, uint32_t value) {
    if(reg < 16)
        r->data[reg]=value;
}

void write_usr_register(registers r, uint8_t reg, uint32_t value) {
    if(reg < 16 && r->mode == USR)
        r->data[reg]=value;
}

void write_cpsr(registers r, uint32_t value) {
    r->data[16]=value;
}
void write_spsr(registers r, uint32_t value) {
    r->data[17]=value;
}
