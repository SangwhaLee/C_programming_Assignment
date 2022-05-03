#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for getopt()


#define BYTES_PER_WORD 4
//#define DEBUG

/*
 * Cache structures
 */
int time = 0;

typedef struct
{
    int age;
    int valid;
    int modified;
    uint32_t tag;
} cline;

typedef struct
{
    cline *lines;
} cset;

typedef struct
{
    int s; // index bits
    int E; // way
    int b; // block offset bits
    cset *sets;
} cache;

int index_bit(int n){
    int cnt = 0;

    while(n) {
        cnt++;
        n = n >> 1;
    }

    return cnt-1;
}

/***************************************************************/
/*                                                             */
/* Procedure : build_cache                                     */
/*                                                             */
/* Purpose   : Initialize cache structure                      */
/*                                                             */
/* Parameters:                                                 */
/*     int S: The set of cache                                 */
/*     int E: The associativity way of cache                   */
/*     int b: The blocksize of cache                           */
/*                                                             */
/***************************************************************/
cache build_cache(int S, int E, int b)
{
    int cap = S * E * b;
    int bnum = cap / b;
    cache temp;
    temp.E = E;
    temp.b = index_bit(b);
    //printf("blockoffset bit is: %d\n", temp.b);
    temp.s = index_bit(bnum/E);
    //printf("index bits id: %d\n", temp.s);
    temp.sets = malloc(sizeof(cset)*S);
    for(int i=0;i<S;i++){
        temp.sets[i].lines = malloc(sizeof(cline)*(cap/b/S));
    }
    //printf("This is set of cache: %d\n", S);
    //printf("This is set of asso: %d\n", E);
    //printf("This is set of blocksize: %d\n", b);
    
	/* Implement this function */
	// return result_cache;
    return temp;
}

/***************************************************************/
/*                                                             */
/* Procedure : access_cache                                    */
/*                                                             */
/* Purpose   : Update cache stat and content                   */
/*                                                             */
/* Parameters:                                                 */
/*     cache *L: An accessed cache                             */
/*     int op: Read/Write operation                            */
/*     uint32_t addr: The address of memory access             */
/*     int *hit: The number of cache hit                       */
/*     int *miss: The number of cache miss                     */
/*     int *wb: The number of write-back                       */
/*                                                             */
/***************************************************************/
void access_cache(cache *L, int op, uint32_t addr, int *readhit,int *writehit, int *readmiss,int *writemiss, int *wb)
{
    unsigned int temp_tag;
    unsigned int temp;
    int temp_set;
    int check=0;
    int max=0;
    int same=0;
    temp_tag = addr >> (L->s + L->b);
    //printf("what is the tag: %u\n", temp_tag);
    temp = addr >> L->b;
    temp_set = temp & ((1 << L->s)-1);
    //printf("what is the set: %u\n", temp_set);
    for(int i=0;i<8;i++){
       if(L->sets[temp_set].lines[i].tag == temp_tag){
           L->sets[temp_set].lines[i].age = 0;
            check =1;
            if(op==1){
                L->sets[temp_set].lines[i].modified =1;
                *writehit = *writehit+1;
            }
            else{
                *readhit = *readhit +1;
            }
            for(int j=0;j<8;j++){
                if(j!=i)
                    L->sets[temp_set].lines[j].age++;
            }
            break;
       }
    }
    if(check != 1){
        if(op ==1){
            *writemiss = *writemiss +1;
        }
        else{
            *readmiss = *readmiss +1;
        }
        for(int i=0; i<8;i++){
            if(L->sets[temp_set].lines[i].valid ==0){
                L->sets[temp_set].lines[i].age=0;
                L->sets[temp_set].lines[i].tag = temp_tag;
                /*for(int k=0;k<1024;k++){
                    if(mem[k]==0){
                        mem[k] = L->sets[temp_set].lines[i].tag;
                        break;
                    }
                }*/
                L->sets[temp_set].lines[i].valid=1;
                check=2;
                if(op==1){
                    L->sets[temp_set].lines[i].modified =1;
                }
                for(int j=0;j<8;j++){
                    if(j!=i)
                        L->sets[temp_set].lines[j].age++;
                }
                break;
            }
        }
        if(check !=2){
            for(int i=0;i<8;i++){
                if(L->sets[temp_set].lines[i].age>max){
                    max = L->sets[temp_set].lines[i].age;
                }
            }
            for(int i=0;i<8;i++){
                if(L->sets[temp_set].lines[i].age == max){
                    L->sets[temp_set].lines[i].age=0;
                    if(op==0){
                        if(L->sets[temp_set].lines[i].modified ==1){
                            //when read, evict the chache block
                            *wb = *wb+1;
                            L->sets[temp_set].lines[i].modified=0; 
                        }
                        L->sets[temp_set].lines[i].tag = temp_tag;
                    }
                    else{
                        if(L->sets[temp_set].lines[i].modified ==1){
                            *wb = *wb+1;
                        }
                        else{
                            L->sets[temp_set].lines[i].modified=1;
                        }
                        L->sets[temp_set].lines[i].tag = temp_tag;
                    }
                    for(int j=0;j<8;j++){
                        if(j!=i)
                            L->sets[temp_set].lines[j].age++;
                    }
                    break;
                }
            }
        }
    }
    //printf("this is the address: %u\n", addr);
	/* Implement this function */
}

/***************************************************************/
/*                                                             */
/* Procedure : cdump                                           */
/*                                                             */
/* Purpose   : Dump cache configuration                        */
/*                                                             */
/***************************************************************/
void cdump(int capacity, int assoc, int blocksize)
{

    printf("Cache Configuration:\n");
    printf("-------------------------------------\n");
    printf("Capacity: %dB\n", capacity);
    printf("Associativity: %dway\n", assoc);
    printf("Block Size: %dB\n", blocksize);
    printf("\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : sdump                                           */
/*                                                             */
/* Purpose   : Dump cache stat                                 */
/*                                                             */
/***************************************************************/
void sdump(int total_reads, int total_writes, int write_backs,
           int reads_hits, int write_hits, int reads_misses, int write_misses)
{
    printf("Cache Stat:\n");
    printf("-------------------------------------\n");
    printf("Total reads: %d\n", total_reads);
    printf("Total writes: %d\n", total_writes);
    printf("Write-backs: %d\n", write_backs);
    printf("Read hits: %d\n", reads_hits);
    printf("Write hits: %d\n", write_hits);
    printf("Read misses: %d\n", reads_misses);
    printf("Write misses: %d\n", write_misses);
    printf("\n");
}


/***************************************************************/
/*                                                             */
/* Procedure : xdump                                           */
/*                                                             */
/* Purpose   : Dump current cache state                        */
/*                                                             */
/* Cache Design                                                */
/*                                                             */
/*      cache[set][assoc][word per block]                      */
/*                                                             */
/*                                                             */
/*       ----------------------------------------              */
/*       I        I  way0  I  way1  I  way2  I                 */
/*       ----------------------------------------              */
/*       I        I  word0 I  word0 I  word0 I                 */
/*       I  set0  I  word1 I  word1 I  work1 I                 */
/*       I        I  word2 I  word2 I  word2 I                 */
/*       I        I  word3 I  word3 I  word3 I                 */
/*       ----------------------------------------              */
/*       I        I  word0 I  word0 I  word0 I                 */
/*       I  set1  I  word1 I  word1 I  work1 I                 */
/*       I        I  word2 I  word2 I  word2 I                 */
/*       I        I  word3 I  word3 I  word3 I                 */
/*       ----------------------------------------              */
/*                                                             */
/*                                                             */
/***************************************************************/
void xdump(cache* L)
{
    int i, j, k = 0;
    int b = L->b, s = L->s;
    int way = L->E, set = 1 << s;

    uint32_t line;

    printf("Cache Content:\n");
    printf("-------------------------------------\n");

    for(i = 0; i < way; i++) {
        if(i == 0) {
            printf("    ");
        }
        printf("      WAY[%d]", i);
    }
    printf("\n");

    for(i = 0; i < set; i++) {
        printf("SET[%d]:   ", i);

        for(j = 0; j < way; j++) {
            if(k != 0 && j == 0) {
                printf("          ");
            }
            if(L->sets[i].lines[j].valid) {
                line = L->sets[i].lines[j].tag << (s + b);
                line = line | (i << b);
            }
            else {
                line = 0;
            }
            printf("0x%08x  ", line);
        }
        printf("\n");
    }
    printf("\n");
}


int main(int argc, char *argv[])
{
    int capacity=1024;
    int way=8;
    int blocksize=8;
    int set;

    // Cache
    cache simCache;
 

    // Counts
    int read=0, write=0, writeback=0;
    int readhit=0, writehit=0;
    int readmiss=0, writemiss = 0;

    // Input option
    int opt = 0;
    char* token;
    int xflag = 0;

    // Parse file
    char *trace_name = (char*)malloc(32);
    FILE *fp;
    char line[16];
    char *op;
    uint32_t addr;

    /* You can define any variables that you want */
    int integer_op;

    trace_name = argv[argc-1];
    //printf("what is trace_name: %s\n", trace_name);
    if (argc < 3) {
        printf("Usage: %s -c cap:assoc:block_size [-x] input_trace \n", argv[0]);
        exit(1);
    }

    while((opt = getopt(argc, argv, "c:x")) != -1) {
        switch(opt) {
            case 'c':
                token = strtok(optarg, ":");
                capacity = atoi(token);
                token = strtok(NULL, ":");
                way = atoi(token);
                token = strtok(NULL, ":");
                blocksize  = atoi(token);
                break;

            case 'x':
                xflag = 1;
                break;

            default:
                printf("Usage: %s -c cap:assoc:block_size [-x] input_trace \n", argv[0]);
                exit(1);

        }
    }

    // Allocate
    set = capacity / way / blocksize;

    /* TODO: Define a cache based on the struct declaration */
    simCache = build_cache(set, way, blocksize);
    //printf("checking: %d, %d, %d\n", simCache.b, simCache.E, simCache.s);

    // Simulate
    fp = fopen(trace_name, "r"); // read trace file
    if(fp == NULL) {
        printf("\nInvalid trace file: %s\n", trace_name);
        return 1;
    }

    cdump(capacity, way, blocksize);

    /* TODO: Build an access function to load and store data from the file */
    while (fgets(line, sizeof(line), fp) != NULL) {
        op = strtok(line, " ");
        addr = strtoull(strtok(NULL, ","), NULL, 16);
        if(!strcmp(op,"R")){
            integer_op = 0;
        }
        else{
            integer_op=1;
        }

#ifdef DEBUG
        // You can use #define DEBUG above for seeing traces of the file.
        fprintf(stderr, "op: %s\n", op);
        fprintf(stderr, "addr: %x\n", addr);
#endif
        // ...
        access_cache(&simCache, integer_op, addr, &readhit, &writehit, &readmiss, &writemiss, &writeback);
        // ...
    }
    read = readhit+ readmiss;
    write = writehit + writemiss;

    // test example
    sdump(read, write, writeback, readhit, writehit, readmiss, writemiss);
    if (xflag) {
        xdump(&simCache);
    }

    return 0;
}
