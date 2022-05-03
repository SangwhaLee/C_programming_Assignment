/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   run.c                                                     */
/*   Adapted from CS311@KAIST                                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>

#include "util.h"
#include "run.h"

/***************************************************************/
/*                                                             */
/* Procedure: get_inst_info                                    */
/*                                                             */
/* Purpose: Read insturction information                       */
/*                                                             */
/***************************************************************/
instruction* get_inst_info(uint32_t pc)
{
    return &INST_INFO[(pc - MEM_TEXT_START) >> 2];
}

/***************************************************************/
/*                                                             */
/* Procedure: process_instruction                              */
/*                                                             */
/* Purpose: Process one instrction                             */
/*                                                             */
/***************************************************************/
void process_instruction()
{
    unsigned int address;
    unsigned int index = (CURRENT_STATE.PC - MEM_TEXT_START)/4;
    //printf("INST_COUNT and NUM_INST : %d, %d\n", INSTRUCTION_COUNT, NUM_INST);
    CURRENT_STATE.PC += 4;
    //printf("check index: %u\n", index);
    if(INST_INFO[index].opcode ==0){
        if(INST_INFO[index].func_code == 36){//and
            CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.r_i.r.rd] =  CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs] & CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt];
        }
        else if(INST_INFO[index].func_code == 33){//addu
            CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs] + CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt];
        }
        else if(INST_INFO[index].func_code == 8){//jr
            CURRENT_STATE.PC = CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs];
        }
        else if(INST_INFO[index].func_code == 0x27){//NOR
            CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.r_i.r.rd] = ~(CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs] | CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt]);
        }
        else if(INST_INFO[index].func_code == 0x25){//OR
            CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.r_i.r.rd] = (CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs] | CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt]);
        }
        else if(INST_INFO[index].func_code == 0x2b){//sltu
            if(CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs] < CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt])
                CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.r_i.r.rd] = 1;
            else{
                CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.r_i.r.rd] = 0;
            }
        }
        else if(INST_INFO[index].func_code == 0x0){//sll
            CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt] << INST_INFO[index].r_t.r_i.r_i.r.shamt;
        }
        else if(INST_INFO[index].func_code == 0x02){//srl
            CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt] >> INST_INFO[index].r_t.r_i.r_i.r.shamt;
        }
        else{//subu
            CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs] - CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt];
        }
    }
    else if(INST_INFO[index].opcode == 2 || INST_INFO[index].opcode ==3){
        if(INST_INFO[index].opcode==2){//j
            CURRENT_STATE.PC = INST_INFO[index].r_t.target*4;
        }
        else{//jal
            CURRENT_STATE.REGS[31] = CURRENT_STATE.PC+4;
            CURRENT_STATE.PC = INST_INFO[index].r_t.target*4;
        }
    }
    else if(INST_INFO[index].opcode==9 || INST_INFO[index].opcode==0xc || INST_INFO[index].opcode==4 || INST_INFO[index].opcode==5 || INST_INFO[index].opcode==0xf || INST_INFO[index].opcode==0x23 || INST_INFO[index].opcode==0xd || INST_INFO[index].opcode==0xb || INST_INFO[index].opcode==0x2b){
        if(INST_INFO[index].opcode==9){//addiu
            CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt] = CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs] + INST_INFO[index].r_t.r_i.r_i.imm;
        }
        else if(INST_INFO[index].opcode==0xc){//andi
            CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt] = CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs] & INST_INFO[index].r_t.r_i.r_i.imm;
        }
        else if(INST_INFO[index].opcode==4){//beq
            if(CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs] == CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt]){
                CURRENT_STATE.PC = CURRENT_STATE.PC+ INST_INFO[index].r_t.r_i.r_i.imm*4;
            }
        }
        else if(INST_INFO[index].opcode==5){//bne
            if(CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs] != CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt]){
                //printf("check for bne imm: %d\n", INST_INFO[index].r_t.r_i.r_i.imm);
                CURRENT_STATE.PC = CURRENT_STATE.PC+ INST_INFO[index].r_t.r_i.r_i.imm*4;
            }
        }
        else if(INST_INFO[index].opcode==0xf){//lui
            //printf("check the lui imm: %d\n", INST_INFO[index].r_t.r_i.r_i.imm);
            CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt] = INST_INFO[index].r_t.r_i.r_i.imm << 16;
        }
        else if(INST_INFO[index].opcode==0x23){//lw
            address = mem_read_32(CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs]+ INST_INFO[index].r_t.r_i.r_i.imm);
            CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt] = address; 
        }   
        else if(INST_INFO[index].opcode==0xd){//ori
            //printf("rs, ori imm: %u, %d\n",CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs] ,INST_INFO[index].r_t.r_i.r_i.imm);
            CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt] = (CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs] | INST_INFO[index].r_t.r_i.r_i.imm);
        }
        else if(INST_INFO[index].opcode==0xb){//sltiu
            if(CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs]<INST_INFO[index].r_t.r_i.r_i.imm){
                CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt] = 1;
            }
            else{
                CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt] = 0;
            }
        }
        else{//sw
            address = CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rs] + INST_INFO[index].r_t.r_i.r_i.imm;
            mem_write_32(address, CURRENT_STATE.REGS[INST_INFO[index].r_t.r_i.rt]);
        }
    }
    if(CURRENT_STATE.PC > MEM_TEXT_START + NUM_INST*4){
        CURRENT_STATE.PC = MEM_TEXT_START + NUM_INST*4;
    }

	/** Implement this function */
}
