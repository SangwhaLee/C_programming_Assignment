#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>

/*
 * For debug option. If you want to debug, set 1.
 * If not, set 0.
 */
#define DEBUG 0

#define MAX_SYMBOL_TABLE_SIZE   1024
#define MEM_TEXT_START          0x00400000
#define MEM_DATA_START          0x10000000
#define BYTES_PER_WORD          4
#define INST_LIST_LEN           20

/******************************************************
 * Structure Declaration
 *******************************************************/

typedef struct inst_struct {
    char *name;
    char *op;
    char type;
    char *funct;
} inst_t;

typedef struct symbol_struct {
    char name[32];
    uint32_t address;
} symbol_t;

enum section { 
    DATA = 0,
    TEXT,
    MAX_SIZE
};

/******************************************************
 * Global Variable Declaration
 *******************************************************/

inst_t inst_list[INST_LIST_LEN] = {       //  idx
    {"addiu",   "001001", 'I', ""},       //    0
    {"addu",    "000000", 'R', "100001"}, //    1
    {"and",     "000000", 'R', "100100"}, //    2
    {"andi",    "001100", 'I', ""},       //    3
    {"beq",     "000100", 'I', ""},       //    4
    {"bne",     "000101", 'I', ""},       //    5
    {"j",       "000010", 'J', ""},       //    6
    {"jal",     "000011", 'J', ""},       //    7
    {"jr",      "000000", 'R', "001000"}, //    8
    {"lui",     "001111", 'I', ""},       //    9
    {"lw",      "100011", 'I', ""},       //   10
    {"nor",     "000000", 'R', "100111"}, //   11
    {"or",      "000000", 'R', "100101"}, //   12
    {"ori",     "001101", 'I', ""},       //   13
    {"sltiu",   "001011", 'I', ""},       //   14
    {"sltu",    "000000", 'R', "101011"}, //   15
    {"sll",     "000000", 'R', "000000"}, //   16
    {"srl",     "000000", 'R', "000010"}, //   17
    {"sw",      "101011", 'I', ""},       //   18
    {"subu",    "000000", 'R', "100011"}  //   19
};

symbol_t SYMBOL_TABLE[MAX_SYMBOL_TABLE_SIZE]; // Global Symbol Table

uint32_t symbol_table_cur_index = 0; // For indexing of symbol table

/* Temporary file stream pointers */
FILE *data_seg;
FILE *text_seg;

/* Size of each section */
uint32_t data_section_size = 0;
uint32_t text_section_size = 0;

/******************************************************
 * Function Declaration
 *******************************************************/

/* Change file extension from ".s" to ".o" */
char* change_file_ext(char *str) {
    char *dot = strrchr(str, '.');

    if (!dot || dot == str || (strcmp(dot, ".s") != 0))
        return NULL;

    str[strlen(str) - 1] = 'o';
    return "";
}

/* Add symbol to global symbol table */
void symbol_table_add_entry(symbol_t symbol)
{
    SYMBOL_TABLE[symbol_table_cur_index++] = symbol;
#if DEBUG
    printf("%s: 0x%08x\n", symbol.name, symbol.address);
#endif
}

/* Convert integer number to binary string */
char* num_to_bits(unsigned int num, int len) 
{
    char* bits = (char *) malloc(len+1);
    int idx = len-1, i;
    while (num > 0 && idx >= 0) {
        if (num % 2 == 1) {
            bits[idx--] = '1';
        } else {
            bits[idx--] = '0';
        }
        num /= 2;
    }
    for (i = idx; i >= 0; i--){
        bits[i] = '0';
    }
    bits[len] = '\0';
    return bits;
}

/* Record .text section to output file */
void record_text_section(FILE *output) 
{
    uint32_t cur_addr = MEM_TEXT_START;
    char line[1024];
    int index=0;

    /* Point to text_seg stream */
    rewind(text_seg);

    /* Print .text section */
    while (fgets(line, 1024, text_seg) != NULL) {
        char *arr;
        char inst[0x1000] = {0};
        char op[32] = {0};
        char label[32] = {0};
        char type = '0';
        int i, idx = 0;
        int rs, rt, rd, imm, shamt;
        int addr;

        rs = rt = rd = imm = shamt = addr = 0;
#if DEBUG
        printf("0x%08x: ", cur_addr);
#endif
        /* Find the instruction type that matches the line */
        /* blank */
        char *temp = (char*)malloc(sizeof(char)*strlen(line));
        line[strlen(line)-1] = '\0';
        strcpy(temp, line);
        temp = strtok(temp, "\t");
        for(int i=0;i<20;i++){
            if(!strcmp(inst_list[i].name,temp)){
                type = inst_list[i].type;
                break;
            }
        }

        switch (type) {
            case 'R':
                /* blank */
                 if(!strcmp(temp,"jr")){
                     strcpy(temp, line);
                     strcpy(op, inst_list[8].op);
                     strcpy(label, inst_list[8].funct);
                     rs =0;
                     rt=0;
                     rd=0;
                     shamt=0;
                     arr = strtok(temp, "$");
                     arr = strtok(NULL, "$");
                     imm = atoi(arr);
                     strcpy(inst, op);
                     arr = num_to_bits(rs, 5);
                     strcat(inst, arr);
                     arr = num_to_bits(rt, 5);
                     strcat(inst, arr);
                     arr = num_to_bits(rd, 5);
                     strcat(inst, arr);
                     arr = num_to_bits(shamt, 5);
                     strcat(inst, arr);
                     strcat(inst, label);
                     inst[32]='\n';
                     inst[33] = '\0';
                     fputs(inst, output);
                 }
                 else if(!strcmp(temp,"sll") || !strcmp(temp,"srl")){
                    if(!strcmp(temp,"sll")){
                        strcpy(op, inst_list[16].op);
                        strcpy(label, inst_list[16].funct);
                    }
                    else{
                        strcpy(op, inst_list[17].op);
                        strcpy(label, inst_list[17].funct);
                    }
                    rs=0;
                    shamt=0;
                    strcpy(temp, line);
                    arr = strtok(temp,"$");
                    arr = strtok(NULL, ",");
                    rd = atoi(arr);
                    arr = strtok(NULL, "$");
                    arr = strtok(NULL, ",");
                    rt = atoi(arr);
                    arr = strtok(NULL," ");
                    arr = strtok(NULL," ");
                    imm = atoi(arr);
                    if(imm<0)
                        imm = 4096+imm;
                    strcpy(inst, op);
                    arr = num_to_bits(rs, 5);
                    strcat(inst, arr);
                    arr = num_to_bits(rt, 5);
                    strcat(inst, arr);
                    arr = num_to_bits(rd, 5);
                    strcat(inst, arr);
                    arr = num_to_bits(shamt, 5);
                    strcat(inst, arr);
                    strcat(inst, label);
                    inst[32]='\n';
                    inst[33] = '\0';
                    fputs(inst, output);
                 }
                 else if(!strcmp(temp,"addu") || !strcmp(temp,"subu") || !strcmp(temp,"sltu")){
                    if(!strcmp(temp, "addu")){
                        strcpy(op, inst_list[1].op);
                        strcpy(label, inst_list[1].funct);
                    }
                    else if(!strcmp(temp,"subu")){
                        strcpy(op, inst_list[19].op);
                        strcpy(label, inst_list[19].funct);
                    }
                    else{
                        strcpy(op, inst_list[15].op);
                        strcpy(label, inst_list[15].funct);
                    }
                     strcpy(temp, line);
                        arr = strtok(temp, "$");
                        arr = strtok(NULL, ",");
                        rd = atoi(arr);
                        arr = strtok(NULL, "$");
                        arr = strtok(NULL, ",");
                        rs =  atoi(arr);
                        arr = strtok(NULL,"$");
                        arr = strtok(NULL, "$");
                        rt = atoi(arr);
                        strcpy(inst, op);
                        arr = num_to_bits(rd, 5);
                        strcat(inst, arr);
                        arr = num_to_bits(rs, 5);
                        strcat(inst,arr);
                        arr = num_to_bits(rt, 5);
                        strcat(inst, arr);
                        strcat(inst, label);
                        inst[32]='\n';
                        inst[33] = '\0';
                        fputs(inst, output);
                 }
                 else{
                     if(!strcmp(temp,"and")){
                        strcpy(op, inst_list[2].op);
                        strcpy(label, inst_list[2].funct);
                     }
                     else if(!strcmp(temp,"nor")){
                        strcpy(op, inst_list[11].op);
                        strcpy(label, inst_list[11].funct);
                     }
                     else{
                        strcpy(op, inst_list[12].op);
                        strcpy(label, inst_list[12].funct);
                     }
                        strcpy(temp, line);
                        arr = strtok(temp, "$");
                        arr = strtok(NULL, ",");
                        rd = atoi(arr);
                        arr = strtok(NULL, "$");
                        arr = strtok(NULL, ",");
                        rs =  atoi(arr);
                        arr = strtok(NULL,"$");
                        arr = strtok(NULL, "$");
                        rt = atoi(arr);
                        strcpy(inst, op);
                        arr = num_to_bits(rd, 5);
                        strcat(inst, arr);
                        arr = num_to_bits(rs, 5);
                        strcat(inst,arr);
                        arr = num_to_bits(rt, 5);
                        strcat(inst, arr);
                        strcat(inst, label);
                        inst[32]='\n';
                        inst[33] = '\0';
                        fputs(inst, output);
                 }
                 index++;
#if DEBUG
                printf("op:%s rs:$%d rt:$%d rd:$%d shamt:%d funct:%s\n",
                        op, rs, rt, rd, shamt, inst_list[idx].funct);
#endif
                break;

            case 'I':
                /* blank */
                
                if(!strcmp(temp,"lui")){
                    strcpy(op, inst_list[9].op);
                    rs=0;
                    strcpy(temp, line);
                    arr = strtok(temp, "$");
                    arr = strtok(NULL, ",");
                    rt = atoi(arr);
                    strcpy(temp, line);
                    arr =strchr(temp, '$');
                    arr = strtok(arr, " ");
                    arr = strtok(NULL, "\n");
                    if(atoi(arr)){
                        imm = atoi(arr);
                    }
                    else   
                        imm = strtol(arr, NULL, 16);
                    strcpy(inst, op);
                    arr = num_to_bits(rs, 5);
                    strcat(inst, arr);
                    arr = num_to_bits(rt, 5);
                    strcat(inst, arr);
                    arr = num_to_bits(imm, 16);
                    strcat(inst, arr);
                    inst[32] = '\n';

                    fputs(inst, output);
                }
                else if(!strcmp(temp,"ori")){
                    strcpy(op, inst_list[13].op);
                    strcpy(temp, line);
                    arr = strtok(temp, "$");
                    arr = strtok(NULL, " ");
                    rt = atoi(arr);
                    arr = strtok(temp, "$");
                    arr = strtok(NULL, " ");
                    rs = atoi(arr);
                    arr = strtok(NULL, " ");
                    if(!atoi(arr)){
                        imm = strtol(arr, NULL, 16);
                    }
                    else{
                        imm = atoi(arr);
                    }
                    strcpy(inst, op);
                    arr = num_to_bits(rs, 5);
                    strcat(inst, arr);
                    arr = num_to_bits(rt, 5);
                    strcat(inst, arr);
                    arr = num_to_bits(imm, 16);
                    strcat(inst, arr);
                    inst[32] = '\n';
                    inst[33] = '\0';
                    fputs(inst, output);
                }
                else if(!strcmp(temp,"beq") || !strcmp(temp,"bne")){
                    if(!strcmp(temp, "beq")){
                        strcpy(op, inst_list[4].op);
                    }
                    else{
                        strcpy(op, inst_list[5].op);
                    }
                    strcpy(temp, line);
                    arr = strtok(temp, "$");
                    arr = strtok(NULL, ",");
                    rs = atoi(arr);
                    arr = strtok(NULL, "$");
                    arr = strtok(NULL, ",");
                    rt = atoi(arr);
                    arr = strtok(NULL, " ");
                    arr = strtok(NULL, " ");
                    for(i=0;i<symbol_table_cur_index;i++){
                        if(!strcmp(SYMBOL_TABLE[i].name, arr)){
                            addr = (0x400000+index*4)-SYMBOL_TABLE[i].address;
                            imm = addr/8;
                            break;
                        }
                    }
                    strcpy(inst, op);
                    arr =num_to_bits(rs, 5);
                    strcat(inst,arr);
                    arr = num_to_bits(rt, 5);
                    strcat(inst, arr);
                    arr = num_to_bits(imm, 16);
                    strcat(inst, arr);
                    inst[32]='\n';
                    inst[33] = '\0';
                    fputs(inst, output);
                }
                else if(!strcmp(temp,"lw")||!strcmp(temp,"sw")){
                    if(!strcmp(temp,"lw")){
                        strcpy(op, inst_list[10].op);
                    }
                    else{
                        strcpy(op, inst_list[18].op);
                    }
                    strcpy(temp, line);
                    arr= strtok(temp, "$");
                    arr = strtok(NULL, ",");
                    rt = atoi(arr);
                    arr = strtok(NULL, " ");
                    arr = strtok(NULL, "(");
                    imm = atoi(arr);
                    arr = strtok(NULL, "$");
                    arr = strtok(NULL, ")");
                    rs = atoi(arr);
                    if(imm<0)
                        imm = 65536+imm;
                    strcpy(inst, op);
                    arr = num_to_bits(rs, 5);
                    strcat(inst, arr);
                    arr = num_to_bits(rt, 5);
                    strcat(inst, arr);
                    arr = num_to_bits(imm, 16);
                    strcat(inst, arr);
                    inst[32] = '\n';
                    inst[33] = '\0';
                    fputs(inst, output);
                }
                else if(!strcmp(temp,"addiu") || !strcmp(temp,"sltiu")){
                    if(!strcmp(temp, "addiu")){
                        strcpy(op, inst_list[0].op);
                    }
                    else{
                        strcpy(op, inst_list[14].op);
                    }
                    strcpy(temp, line);
                    arr = strtok(temp, "$");
                    arr = strtok(NULL, ",");
                    rt = atoi(arr);
                    arr = strtok(NULL, "$");
                    arr = strtok(NULL, ",");
                    rs = atoi(arr);
                    arr = strtok(NULL, " ");
                    arr = strtok(NULL, " ");
                    imm = atoi(arr);
                    if(imm<0)
                        imm = 65536+imm;
                    strcpy(inst, op);
                    arr = num_to_bits(rs, 5);
                    strcat(inst, arr);
                    arr = num_to_bits(rt, 5);
                    strcat(inst, arr);
                    arr = num_to_bits(imm, 16);
                    strcat(inst, arr);
                    inst[32]='\n';
                    inst[33] = '\0';
                    fputs(inst,output);
                }
                else{
                    strcpy(op, inst_list[3].op);
                    strcpy(temp,line);
                    arr= strtok(temp, "$");
                    arr = strtok(NULL, ",");
                    rt = atoi(arr);
                    arr = strtok(NULL, "$");
                    arr = strtok(NULL, ",");
                    rs =atoi(arr);
                    arr = strtok(NULL, " ");
                    arr = strtok(NULL, " ");
                    if(!atoi(arr)){
                        imm = strtol(arr, NULL, 16);
                    }
                    else   
                        imm = atoi(arr);
                    if(imm<0)
                        imm = imm+ 65536;
                    strcpy(inst, op);
                    arr = num_to_bits(rs, 5);
                    strcat(inst, arr);
                    arr = num_to_bits(rt, 5);
                    strcat(inst, arr);
                    arr = num_to_bits(imm, 16);
                    strcat(inst, arr);
                    inst[32] = '\n';
                    inst[33] = '\0';
                    fputs(inst, output);
                }
                index++;
#if DEBUG
                printf("op:%s rs:$%d rt:$%d imm:0x%x\n",
                        op, rs, rt, imm);
#endif
                break;

            case 'J':
                if(!strcmp(temp, "j")){
                    strcpy(op, inst_list[6].op);
                    strcpy(temp,line);
                    arr = strtok(temp, "\t");
                    arr = strtok(NULL, "\t");
                    for(i=0;i<symbol_table_cur_index;i++){
                        if(!strcmp(SYMBOL_TABLE[i].name, arr)){
                            addr = SYMBOL_TABLE[i].address;
                            imm = addr/4;
                            break;
                        }
                    }
                    strcpy(inst, op);
                    arr = num_to_bits(imm, 26);
                    strcat(inst, arr);
                    inst[32] = '\n';
                    inst[33] = '\0';
                    fputs(inst,output);
                }
                else{
                    strcpy(op, inst_list[7].op);
                    strcpy(temp,line);
                    arr = strtok(temp, "\t");
                    arr = strtok(NULL, "\t");
                    for(i=0;i<symbol_table_cur_index;i++){
                        if(!strcmp(SYMBOL_TABLE[i].name, arr)){
                            addr = SYMBOL_TABLE[i].address;
                            imm = addr/4;
                            break;
                        }
                    }
                    strcpy(inst, op);
                    arr = num_to_bits(imm, 26);
                    strcat(inst, arr);
                    inst[32] = '\n';
                    inst[33] = '\0';
                    fputs(inst,output);
                }
                /* blank */

#if DEBUG
                printf("op:%s addr:%i\n", op, addr);
#endif
                free(temp);
                index++;
                break;

            default:
                break;
        }
        fprintf(output, "\n");

        cur_addr += BYTES_PER_WORD;
    }
}

/* Record .data section to output file */
void record_data_section(FILE *output)
{
    uint32_t cur_addr = MEM_DATA_START;
    char line[1024];
    char* temp;
    char arr[1024];
    int num;
    /* Point to data segment stream */
    rewind(data_seg);

    /* Print .data section */
    while (fgets(line, 1024, data_seg) != NULL) {
        /* blank */
        temp = strtok(line, "\t");
        temp = strtok(NULL, "\t");
        if(!atoi(temp)){
            num = strtol(temp, NULL, 16);
        }
        else{
            num = atoi(temp);
        }
        temp = num_to_bits(num, 32);
        strcpy(arr, temp);
        arr[32]= '\n';
        arr[33] = '\0';
        fputs(temp, output);

#if DEBUG
        printf("0x%08x: ", cur_addr);
        printf("%s", line);
#endif
        cur_addr += BYTES_PER_WORD;
    }
}

/* Fill the blanks */
void make_binary_file(FILE *output)
{
#if DEBUG
    char line[1024] = {0};
    rewind(text_seg);
    /* Print line of text segment */
    while (fgets(line, 1024, text_seg) != NULL) {
        printf("%s",line);
    }
    printf("text section size: %d, data section size: %d\n",
            text_section_size, data_section_size);
#endif

    /* Print text section size and data section size */
    int data_num;
    int text_num;
    char* data_bi;
    char* text_bi;
    char data_arr[1024];
    char text_arr[1024];

    int temp;
    int len;
    
    data_num = data_section_size*4;
    text_num = text_section_size*4;

    temp = data_num;
    data_bi = num_to_bits(data_num, 32);
    temp = text_num;
    text_bi = num_to_bits(text_num, 32);
    strcpy(data_arr, data_bi);
    strcpy(text_arr, text_bi);
    data_arr[32] = '\n';
    data_arr[33] = '\0';
    text_arr[32] = '\n';
    text_arr[33] = '\0';
    fputs(text_bi, output);
    fputs(data_bi, output);

    /* blank */

    /* Print .text section */
    record_text_section(output);

    /* Print .data section */
    record_data_section(output);
}

/* Fill the blanks */
void make_symbol_table(FILE *input)
{
    char line[1024] = {0};
    uint32_t address = 0;
    enum section cur_section = MAX_SIZE;

    /* Read each section and put the stream */
    while (fgets(line, 1024, input) != NULL) {
        char *temp;
        char *temp_2;
        char *temp_3;
        char* lui = (char*)malloc(sizeof(char)*INST_LIST_LEN);
        char* ori = (char*)malloc(sizeof(char)*INST_LIST_LEN);
        strcpy(lui, "lui");
        strcpy(ori, "ori");
        symbol_t* symbol_temp;
        char _line[1024] = {0};
        strcpy(_line, line);
        temp = strtok(_line, "\t\n");
        temp_2 = temp;

        /* Check section type */
        if (!strcmp(temp, ".data")) {
            /* blank */
            address = 0x10000000;
            cur_section = DATA;
            data_seg = tmpfile();
            continue;
        }
        else if (!strcmp(temp, ".text")) {
            /* blank */
            address = 0x00400000;
            cur_section = TEXT;
            text_seg = tmpfile();
            continue;
        }

        /* Put the line into each segment stream */
        if (cur_section == DATA) {
            /* blank */
            temp_2 = strstr(line,".word");
            data_section_size++;
            if(temp_2 != NULL){
                fwrite(temp_2, sizeof(char),1024, data_seg);
                printf("%s", temp_2);
            }
        }
        else if (cur_section == TEXT) {
            /* blank */
            temp_2 = strchr(temp, ':');
            printf("%s", temp_2);
            if(temp_2 ==NULL){
                text_section_size++;
                temp_2 = strstr(temp, "la\t");
                if(temp_2 !=NULL){
                    temp_3 = (char*)malloc(sizeof(char)*INST_LIST_LEN);
                    strcpy(temp_3,temp);
                    temp_2 = strstr(temp_3, "la\t");
                    temp_3 = strtok(temp_3, " ");
                    temp_3 = strtok(NULL, " ");
                    for(int i=0;i<symbol_table_cur_index;i++){
                        if(!strcmp(SYMBOL_TABLE[i].name, temp_3)){
                            temp_2 = strtok(temp,"\t");
                            temp_2 = strtok(NULL, "\n");
                            temp[strlen(temp)] = '\n';
                            temp = strstr(temp,"\t$");
                            if(SYMBOL_TABLE[i].address%0x10000!=0){
                                text_section_size++;
                                lui = strcat(lui, temp);
                                ori = strcat(ori, temp);
                                fputs(lui, text_seg);
                                fputs(ori, text_seg);
                                address += 2*BYTES_PER_WORD;
                                break;
                            }
                            else{
                                lui = strcat(lui,temp);
                                fputs(lui,text_seg);
                                address += BYTES_PER_WORD;
                                break;
                            }
                        }
                    }
                    free(temp_3);
                    continue;
                }
                temp_2 = strtok(temp,"\t");
                temp_2 = strtok(NULL, "\n");
                temp[strlen(temp)]='\n';
                fputs(temp, text_seg);
            }
        }

        temp_2=strchr(temp,':');
        if(temp_2 != NULL){
            temp = strtok(temp, ":");
            symbol_temp = (symbol_t*)malloc(sizeof(symbol_t));
            strcpy(symbol_temp->name,temp);
            symbol_temp->address = address;
            symbol_table_add_entry(*symbol_temp);
            if(cur_section == TEXT)
                address  -= BYTES_PER_WORD;
        }

        address += BYTES_PER_WORD;
        free(lui);
        free(ori);
    }
}

/******************************************************
 * Function: main
 *
 * Parameters:
 *  int
 *      argc: the number of argument
 *  char*
 *      argv[]: array of a sting argument
 *
 * Return:
 *  return success exit value
 *
 * Info:
 *  The typical main function in C language.
 *  It reads system arguments from terminal (or commands)
 *  and parse an assembly file(*.s).
 *  Then, it converts a certain instruction into
 *  object code which is basically binary code.
 *
 *******************************************************/

int main(int argc, char* argv[])
{
    FILE *input, *output;
    char *filename;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <*.s>\n", argv[0]);
        fprintf(stderr, "Example: %s sample_input/example?.s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Read the input file */
    input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    /* Create the output file (*.o) */
    filename = strdup(argv[1]); // strdup() is not a standard C library but fairy used a lot.
    if(change_file_ext(filename) == NULL) {
        fprintf(stderr, "'%s' file is not an assembly file.\n", filename);
        exit(EXIT_FAILURE);
    }

    output = fopen(filename, "w");
    if (output == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    /******************************************************
     *  Let's complete the below functions!
     *
     *  make_symbol_table(FILE *input)
     *  make_binary_file(FILE *output)
     *  ├── record_text_section(FILE *output)
     *  └── record_data_section(FILE *output)
     *
     *******************************************************/
    make_symbol_table(input);
    make_binary_file(output);

    fclose(input);
    fclose(output);

    return 0;
}
