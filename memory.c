/*
Armator - simulateur de jeu d'instruction ARMv5T ? but p?dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G?n?rale GNU publi?e par la Free Software
Foundation (version 2 ou bien toute autre version ult?rieure choisie par vous).

Ce programme est distribu? car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp?cifique. Reportez-vous ? la
Licence Publique G?n?rale GNU pour plus de d?tails.

Vous devez avoir re?u une copie de la Licence Publique G?n?rale GNU en m?me
temps que ce programme ; si ce n'est pas le cas, ?crivez ? la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
?tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B?timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H?res
*/
#include <stdlib.h>
#include "memory.h"
#include "util.h"

struct memory_data {
    size_t size;
    int is_big_endian;
    uint8_t *data;
};

memory memory_create(size_t size, int is_big_endian) {
    memory mem=(memory) malloc(sizeof(struct memory_data));
    mem->size=size;
    mem->is_big_endian=is_big_endian;
    mem->data = (uint8_t *) malloc(sizeof(uint8_t)*size);
    return mem;
}

size_t memory_get_size(memory mem) {
    return mem->size;
}

void memory_destroy(memory mem) {
    free(mem->data);
    free(mem);
}

int memory_read_byte(memory mem, uint32_t address, uint8_t *value) {
    if(address >=memory_get_size(mem)) return -1;
    *value = *((uint8_t *) (mem->data+address));
    return 0;
}

int memory_read_half(memory mem, uint32_t address, uint16_t *value) {
    if(address>=memory_get_size(mem)) return -1;
    if(address%2!=0) return -1;
    *value = *((uint16_t *) (mem->data+address));
    if(is_big_endian() != mem->is_big_endian )
        *value = reverse_2(*value);
    return 0;
}

int memory_read_word(memory mem, uint32_t address, uint32_t *value) {
    if(address>=memory_get_size(mem)) return -1;
    if(address%4!=0) return -1;
    *value = * ((uint32_t *) (mem->data+address));
    if(is_big_endian() != mem->is_big_endian )
        *value = reverse_4(*value);
    return 0;
}

int memory_write_byte(memory mem, uint32_t address, uint8_t value) {
    if(address>=memory_get_size(mem)) return -1;
    mem->data[address]=value;
    return 0;
}

int memory_write_half(memory mem, uint32_t address, uint16_t value) {
    if(address>=memory_get_size(mem)) return -1;
    if(address%2!=0) return -1;
    if(is_big_endian() != mem->is_big_endian )
        value = reverse_2(value);
    for(int i = 0; i<2;i++)
        mem->data[address+i] = * ( ((uint8_t *) &value) +i);
    return 0;
}

int memory_write_word(memory mem, uint32_t address, uint32_t value) {
    if(address>=memory_get_size(mem)) return -1;
    if(address%4!=0) return -1;
    if(is_big_endian() != mem->is_big_endian )
        value = reverse_4(value);
    for(int i = 0; i<4;i++)
        mem->data[address+i] = * ( ((uint8_t *) &value) +i);
    return 0;
}
