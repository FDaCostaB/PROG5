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

void updateZN(arm_core p,uint32_t result){

    //indicateur Z
    if(result == 0){
        arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<30)));
    }
    else
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<30))));

    //indicateur N
    if (get_bit(result,31) == 1)
    {
        arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<31)));
    }
    else
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<31))));


}

uint32_t arm_and (arm_core p,uint32_t val1, uint32_t val2 ,uint8_t s){
    uint32_t  result;
    result= val1 & val2;
    if(s){
        arm_tst(p,val1,Val2);
    }
    return result;
}

uint32_t arm_eor (arm_core p,uint32_t val1, uint32_t val2,uint8_t s){
    uint32_t  result;
    result= val1 ^  val2;
    if(s){
        arm_teq(p,val1,val2);
    }
    return result;
}

uint32_t arm_sub (arm_core p,uint32_t val1, uint32_t val2,uint8_t s,uint8_t c,uint8_t v){
    uint32_t  result;
    result= val1 -  val2;
    if(s){
        arm_cmp(p,val1,val2,c,v);
    }
    return result;
}

uint32_t arm_orr(uint32_t val1, uint32_t val2,uint8_t s){
    uint32_t  result;
    result= val1 | val2;
    if(s){
        updateZN(p,result);
    }
    return result;

}
uint32_t arm_add(arm_core p,uint32_t val1, uint32_t val2,uint8_t s,uint8_t c,uint8_t v,uint8_t s){
    uint32_t  result;
    result = val1 + val2;
    if(s){
        arm_cmp(p,val1,val2,c,v);
    }
    return result;
}

uint32_t arm_bic(arm_core p,uint32_t val1, uint32_t val2,uint8_t s){
    uint32_t  result;
    result= val1 & (~val2);
    if(s){
        arm_tst(p,val1,(~val2));
    }
    return result;
}

int arm_mov(arm_core p,uint8_t rd,uint32_t val_2,uint8_t s){
    
    arm_write_register(p,rd,val_2);
    uint32_t result=arm_read_register(p,rd);
    
    if (s && (!strcmp(arm_get_register_name(rd),"PC"))){
        if (arm_current_mode_has_spsr()){
            arm_write_cpsr(p,arm_read_spsr);
        } else {
            return -1;
        }

    }
    //si le registre destination est en mode user
    if(s){
        updateZN(p,result);
    }
    return 0;
}

int arm_mvn(arm_core p,uint8_t rd, uint32_t val,uint8_t s){

    arm_write_register(p,rd, (~val));
    uint32_t result=arm_read_register(p,rd);
    if (s && (!strcmp(arm_get_register_name(rd),"PC"))){
        if (arm_current_mode_has_spsr()){
            arm_write_cpsr(p,arm_read_spsr);
        } else {
            return -1;
        }

    }
    //si le registre de dest est en mode user
    if(s){
        updateZN(p,result);
    }
    return 0;
}

uint32_t arm_sbc(arm_core p,uint32_t val1, uint32_t val2, uint8_t c,uint8_t s){
       uint32_t  result;
       result= val1 - val2 - !c;
        if(s){
        arm_cmp(p,val1,(val2-(~(get_bit(arm_read_cpsr(p),29)))));
        }

    return result;
}

uint32_t arm_adc(arm_core p,uint32_t val1,uint32_t val2, uint8_t c,uint8_t s){
  
      uint32_t result;

      result= val1 + val2 + c;

     if(s){
           arm_cmn(p,val1,(val2+((get_bit(arm_read_cpsr(p),29)))));
     }

    return result;
}

void arm_teq(arm_core p,uint32_t val1,uint32_t val2){

    //Update flags after Rn EOR shifter_operand

    uint32_t result;

    result=val1 ^ val2;

    updateZN(p,result);

    // Indicateur C;
    //C=0 dans un xor
    arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<29))));

}


void arm_tst(arm_core p,uint32_t val1,uint32_t val2){

    uint32_t result;

    result=val1 & val2;

    updateZN(p,result);

    arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<29))));

}

void arm_cmp(arm_core p,uint32_t val1,uint32_t val2, uint8_t  c, uint8_t v){

    uint32_t result;

    result=val1 - val2;


    updateZN(p,result);


    //indicateur V
    if(get_bit(val1,31) == get_bit(val2,31) && get_bit(val1,31) != get_bit(result,31)){
        arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<28)));
    }
    else
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));

    // Indicateur C
    if( (0xffffffff - val1) < ~val2 + 0x1 && !c){
        arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 29));
    }
    if( (0xffffffff - val1) >= ~val2 + 0x1 && c) {
        arm_write_cpsr(p, clr_bit(arm_read_cpsr(p), 29));
    }


}

void arm_cmn(arm_core p,uint32_t  val1, uint32_t val2, uint8_t c){

    //Update flags after Rn + shifter_operand

    uint32_t result;
    result = val1 + val2;
    updateZN(p,result);

    //indicateur V
    if(get_bit(val1,31) == get_bit(val2,31) && get_bit(val1,31) != get_bit(result,31)){

        arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<28)));
    }
    else
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));



    // Indicateur C

    if( (0xffffffff - val1) < val2 && !c){
        arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 29));
    }
    if( (0xffffffff - val1) >= val2 && c) {
        arm_write_cpsr(p, clr_bit(arm_read_cpsr(p), 29));
    }


}

void arm_rsb(arm_core p, uint8_t rd, uint32_t val_1,uint32_t val_2,  uint8_t  s, uint8_t c, uint8_t v){

    //Rd := shifter_operand - Rn
    uint32_t result;

    result= val_1 - val_2;
    arm_write_register(p,rd, result);

    if(s){

        //mis a jour Z et N
        updateZN(p,result);

        //indicateur C
        if( (0xffffffff - val_2) < ~val_1 + 0x1 && !c){
            arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 29));
        }
        if( (0xffffffff - val_2) >= ~val_1 + 0x1 && c) {
            arm_write_cpsr(p, clr_bit(arm_read_cpsr(p), 29));
        }

        //indicateur V
        if(get_bit(val_2,31) == get_bit(val_1,31) && get_bit(val_2,31) != get_bit(result,31)){
            arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<28)));
        }
        else
            arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));
    }



}

void arm_rsc(arm_core p,uint8_t rd,uint32_t val_1,uint32_t val_2,  uint8_t s, uint8_t c){

    uint32_t result;

    result= val_2 - val_1 - !c;
    arm_write_register(p,rd, result);

    if(s){
        updateZN(p,result);

        //indicateur C
        if( (0xffffffff - val_2 - c) < ~val_1 + 0x2 && !c){
            arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 29));
        }
        if( (0xffffffff - val_2 - c) >= ~val_1 + 0x2 && c) {
            arm_write_cpsr(p, clr_bit(arm_read_cpsr(p), 29));
        }

        //indicateur V
        if(get_bit(val_2,31) == get_bit(val_2,31) && get_bit(val_2,31)== (~(get_bit(arm_read_cpsr(p),29))) && get_bit(val_2,31) != get_bit(result,31)){
            arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<28)));
        }
        else
            arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));
    }
}

uint32_t apply_rotation_imm(uint32_t ins){
    uint8_t rotation_imm = (uint8_t)((ins & 0xf00) >> 8);
	uint8_t immed_8 = (uint8_t) (ins & 0xff);
    if (rotation_imm!=0)
        return (uint32_t) ((immed_8 >> rotation_imm*2) | (immed_8 << (8-rotation_imm*2))); //rotation de rotation_imm x 2 bits
    return immed_8;
}
int decode_operand(arm_core p, uint32_t ins, uint32_t *val_1, uint32_t *val_2){ //RRX A IMPLEMENTER
	uint32_t rn = (ins >> 16) & 0xf; //adresse registre source
	uint8_t bit_25 = (uint8_t) get_bit(ins,25);
	uint8_t bit_7 = (uint8_t) get_bit(ins,7);
	uint8_t bit_4 = (uint8_t) get_bit(ins,4);
	
	*val_1 = arm_read_register(p,rn);

	if (bit_25==1){ //32-bit immediate
		*val_2 = apply_rotation_imm(ins);
	} else {
		uint32_t rm = (ins & 0xf);
		uint8_t shift = (uint8_t)((ins >> 5) & 2); //type du shift
		uint8_t val_shift;
		*val_2 = arm_read_register(p,rm);

		if (bit_4==0){ //shift par immediate
			 val_shift = (ins >> 7) & 31; //31 = 11111 
		} else if (bit_7==0){ //shift par registre
			uint32_t rs = (ins >> 8) & 0xf;
			val_shift = (uint8_t) arm_read_register(p,rs);
		} else {
			return -1; //PAS UNE INSTRUCTION DE DATA PROCESSING
		}
		switch(shift){
			case LSL: //LSL
				*val_2 = *val_2 << val_shift;
				break;
			case LSR: //LSR
				*val_2 = *val_2 >> val_shift;
				break;
			case ASR: //ASR
				*val_2 = asr(*val_2,val_shift);
				break;
			case ROR: //ROR ou RRX
				*val_2 = ror(*val_2,val_shift);
				break;
		}
	}
	return 0;
}

/* Decoding functions for different classes of instructions */

int arm_data_processing_shift(arm_core p, uint32_t ins) {
    if(ConditionPassed(arm_read_cpsr(p),ins)){
        uint32_t val_1, val_2, res;
        uint8_t bit_s = get_bit(ins,20);
        uint8_t bit_c = get_bit(arm_read_cpsr(p),29);
        uint8_t bit_v = get_bit(arm_read_cpsr(p),28);
        uint8_t rd = (uint8_t) ((ins >> 12) & 0xf); //adresse registre destination
        int isdataproc = decode_operand(p,ins,&val_1,&val_2);
        if (isdataproc == -1) return UNDEFINED_INSTRUCTION;
        uint8_t opcode = (uint8_t)((ins >> 21) & 0xf);
        switch (opcode){
            case 0:
                res = arm_and(val_1,val_2);
                break;
            case 1:
                res = arm_eor(val_1,val_2);
                break;
            case 2:
                res = arm_sub(val_1,val_2);
                break;
            case 3:
                arm_rsb(p,rd,val_1,val_2,bit_s,bit_c,bit_v);
                return 0;
            case 4:
                res = arm_add(val_1,val_2);
                break;
            case 5:
                res = arm_adc(p,val_1,val_2,bit_c);
                break;
            case 6:
                res = arm_sbc(p,val_1,val_2,bit_c);
                break;
            case 7:
                arm_rsc(p,val_1,rd,val_2,bit_s,bit_c);
                return 0;
            case 8:
                arm_tst(p,val_1,val_2);
                return 0;
            case 9:
                arm_teq(p,val_1,val_2);
                return 0;
            case 10:
                arm_cmp(p,val_1,val_2,bit_c,bit_v);
                return 0;
            case 11:
                arm_cmn(p,val_1,val_2,bit_c);
                return 0;
            case 12:
                res = arm_orr(val_1,val_2);
                break;
            case 13:
                if (arm_mov(p,rd,val_2,bit_s)==0)
                    break;
                return UNDEFINED_INSTRUCTION;
            case 14:
                res = arm_bic(val_1,val_2);
                break;
            case 15:
                if(arm_mvn(p,rd,val_2,bit_s)==0);
                    break;
                return UNDEFINED_INSTRUCTION;
            default:
                return UNDEFINED_INSTRUCTION;
        }
        arm_write_register(p,rd,res);
        return 0;
    }
    return UNDEFINED_INSTRUCTION;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins){
    uint32_t bytemask;
    uint32_t tmp;
    uint32_t tmp2;
    uint32_t tmp3;
    uint32_t tmp4;
    uint32_t val_rotated = apply_rotation_imm(ins); // operand
    uint32_t mask;
    uint8_t bit_r = get_bit(ins,22);
    uint8_t fieldC = get_bit(ins,16);
    uint8_t fieldX = get_bit(ins,17);
    uint8_t fieldS = get_bit(ins,18);
    uint8_t fieldF = get_bit(ins,19);

    if(ConditionPassed(arm_read_cpsr(p),ins)){
        if ((get_bits(ins,23,5)==0b00110) && (get_bits(ins,20,2)==0b10)){
            arm_write_cpsr(p,val_rotated);
        }
        fieldC ? tmp = 0x000000FF : 0x00000000;
        fieldX ? tmp2 = 0x0000FF00 : 0x00000000;
        fieldS ? tmp3 = 0x00FF0000 : 0x00000000;
        fieldF ? tmp4 = 0xFF000000 : 0x00000000; 
        bytemask = tmp | tmp2 | tmp3 | tmp4;
        if (bit_r == 0){
            if(arm_in_a_privileged_mode(p)) {
                if ((val_rotated & StateMask) != 0)
                    return UNDEFINED_INSTRUCTION;
                else{
                    mask = bytemask & (UserMask | PrivMask);
                }
            }
            else{
                mask = bytemask & UserMask;
            }
            arm_write_cpsr(p,((arm_read_cpsr(p) & (~mask)) | (val_rotated & mask)));
            
        } else { //bit_r == 1
            if (arm_current_mode_has_spsr(p)){
                mask = bytemask & (UserMask | PrivMask | StateMask);
                arm_write_spsr(p,((arm_read_spsr(p) & (~mask))|(val_rotated & mask)));
            } else {
                return UNDEFINED_INSTRUCTION;
            }
        }
        
    }
    return UNDEFINED_INSTRUCTION;
}
