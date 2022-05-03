/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   parse.c                                                   */
/*   Adapted from CS311@KAIST                                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>

#include "util.h"
#include "parse.h"

int text_size;
int data_size;

instruction parsing_instr(const char *buffer, const int index)
{
   instruction instr;
    char* arr = malloc(sizeof(char)*32);
    char* arr2 = malloc(sizeof(char)*32);
    int num;
    strcpy(arr, buffer);
    instr.value = fromBinary(arr);
    char* start = arr;
    strncpy(arr2, start, 6);
    num = fromBinary(arr2);
    //printf("opcode: %s  %d\n", arr2, num);
    instr.opcode = (short)num;
    if(instr.opcode == 0){
        strncpy(arr2, start+6, 5);
        arr2[5] = '\0';
        num = fromBinary(arr2);
        //printf("rs: %s  %d\n", arr2, num);
        instr.r_t.r_i.rs = (unsigned char)num;
        strncpy(arr2, start+11, 5);
        arr2[5]= '\0';
        num = fromBinary(arr2);
        //printf("rt: %s  %d\n", arr2, num);
        instr.r_t.r_i.rt = (unsigned char)num;
        strncpy(arr2, start+16, 5);
        arr2[5]= '\0';
        num = fromBinary(arr2);
        //printf("rd: %s  %d\n", arr2, num);
        instr.r_t.r_i.r_i.r.rd = (unsigned char)num;
        strncpy(arr2, start+21, 5);
        arr2[5]= '\0';
        num = fromBinary(arr2);
        //printf("shamt: %s  %d\n", arr2, num);
        instr.r_t.r_i.r_i.r.shamt = (unsigned char)num;
        strncpy(arr2, start+26, 6);
        arr2[6]= '\0';
        num = fromBinary(arr2);
        //printf("funct: %s  %d\n", arr2, num);
        instr.func_code = (short)num;
        //printf("opcode: %d, rs: %d, rt: %d, rd: %d, shamt: %d, funct: %d\n", instr.opcode, instr.r_t.r_i.rs, instr.r_t.r_i.rt, instr.r_t.r_i.r_i.r.rd, instr.r_t.r_i.r_i.r.shamt, instr.func_code);
    }
    else if(instr.opcode == 2 || instr.opcode ==3){
        strncpy(arr2, start+6, 26);
        arr2[26] = '\0';
        num = fromBinary(arr2);
        //printf("target: %s  %d\n", arr2, num);
        instr.r_t.target = (unsigned int)num;
        //printf("opcode: %d, taeget: %u\n", instr.opcode, instr.r_t.target);
    }
    else{
        strncpy(arr2, start+6, 5);
        arr2[5] = '\0';
        num = fromBinary(arr2);
        //printf("rs: %s  %d\n", arr2, num);
        instr.r_t.r_i.rs = (unsigned char)num;
        strncpy(arr2, start+11, 5);
        arr2[5] = '\0';
        num = fromBinary(arr2);
        //printf("rt: %s  %d\n", arr2, num);
        instr.r_t.r_i.rt = (unsigned char)num;
        if(instr.opcode == 0x9 || instr.opcode == 0x4 || instr.opcode == 0x5 || instr.opcode == 0x23 || instr.opcode == 0xb || instr.opcode == 0x2b){   
            strncpy(arr2, start+16, 16);
            arr2[16] = '\0';
            if(arr2[0]=='1'){
                num = fromBinary(arr2) - 65536;
                //printf("imm: %s  %d\n", arr2, num);
                instr.r_t.r_i.r_i.imm = (short)num;
            }
            else{
                strncpy(arr2, start+16, 16);
                arr2[16] = '\0';
                num = fromBinary(arr2);
                //printf("imm: %s  %d\n", arr2, num);
                instr.r_t.r_i.r_i.imm = (short)num;
            }
        }
        else{
            strncpy(arr2, start+16, 16);
            arr2[16] = '\0';
            num = fromBinary(arr2);
            //printf("imm: %s  %d\n", arr2, num);
            instr.r_t.r_i.r_i.imm = (short)num;
        }
        //printf("opcode: %d, rs: %d, rt: %d, imm: %d\n", instr.opcode,instr.r_t.r_i.rs, instr.r_t.r_i.rt, instr.r_t.r_i.r_i.imm);
    }
    
	/** Implement this function */
    return instr;
}

void parsing_data(const char *buffer, const int index)
{
    //printf("what is data: %s\n", buffer);
	/** Implement this function */
    unsigned int num;
    unsigned int address = (unsigned int)index + MEM_DATA_START;
    char* text = malloc(sizeof(char)*32);
    strcpy(text, buffer);
    num = (unsigned int)fromBinary(text);
    //printf("address: %u\nnum: %u\n", address, num);
    mem_write_32(address, num);
}

void print_parse_result()
{
    int i;
    printf("Instruction Information\n");

    for(i = 0; i < text_size/4; i++)
    {
        printf("INST_INFO[%d].value : %x\n",i, INST_INFO[i].value);
        printf("INST_INFO[%d].opcode : %d\n",i, INST_INFO[i].opcode);

	    switch(INST_INFO[i].opcode)
        {
            //Type I
            case 0x9:		//(0x001001)ADDIU
            case 0xc:		//(0x001100)ANDI
            case 0xf:		//(0x001111)LUI	
            case 0xd:		//(0x001101)ORI
            case 0xb:		//(0x001011)SLTIU
            case 0x23:		//(0x100011)LW
            case 0x2b:		//(0x101011)SW
            case 0x4:		//(0x000100)BEQ
            case 0x5:		//(0x000101)BNE
                printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
                printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
                printf("INST_INFO[%d].imm : %d\n",i, INST_INFO[i].r_t.r_i.r_i.imm);
                break;

            //TYPE R
            case 0x0:		//(0x000000)ADDU, AND, NOR, OR, SLTU, SLL, SRL, SUBU  if JR
                printf("INST_INFO[%d].func_code : %d\n",i, INST_INFO[i].func_code);
                printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
                printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
                printf("INST_INFO[%d].rd : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.rd);
                printf("INST_INFO[%d].shamt : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.shamt);
                break;

            //TYPE J
            case 0x2:		//(0x000010)J
            case 0x3:		//(0x000011)JAL
                printf("INST_INFO[%d].target : %d\n",i, INST_INFO[i].r_t.target);
                break;

            default:
                printf("Not available instruction\n");
                assert(0);
        }
    }

    printf("Memory Dump - Text Segment\n");
    for(i = 0; i < text_size; i+=4)
        printf("text_seg[%d] : %x\n", i, mem_read_32(MEM_TEXT_START + i));
    for(i = 0; i < data_size; i+=4)
        printf("data_seg[%d] : %x\n", i, mem_read_32(MEM_DATA_START + i));
    printf("Current PC: %x\n", CURRENT_STATE.PC);
}
