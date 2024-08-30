#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "lib.h"

#define MEMORY_SIZE 0x0100

    int vector[8] = {0};
    int pc = 0;
    int teste1 = 0; 
    int teste2 = 0;
    int isRunning = 1;

void fetch(uint16_t *memory, int pc, uint16_t *instrucao);
void decode(uint16_t instrucao, int *formato, int *opcodeR, int *destino, int *ope1, int *ope2, int *opcodeI, int *registrador, int *imediato);
void execute(int formato, int opcodeR, int destino, int ope1, int ope2, int opcodeI, int registrador, int imediato, int *pc_increment);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: %s [bin_name]\n", argv[0]);
        exit(1);
    }

    uint16_t memory[MEMORY_SIZE] = {0};
    load_binary_to_memory(argv[1], memory, MEMORY_SIZE);

    while (isRunning) {
        uint16_t instrucao;
        fetch(memory, pc, &instrucao);

        int formato, opcodeR, destino, ope1, ope2, opcodeI, registrador, imediato;
        decode(instrucao, &formato, &opcodeR, &destino, &ope1, &ope2, &opcodeI, &registrador, &imediato);

        int pc_increment = 1;

        execute(formato, opcodeR, destino, ope1, ope2, opcodeI, registrador, imediato, &pc_increment);

        if (pc_increment) {
            pc++;
        }

        if (pc >= MEMORY_SIZE) {
            isRunning = 0;
        }
    }

    for(int x = 0; x < 8; x++) {
        printf("Vetor[%d]: %d\n", x, vector[x]);
    }
    return 0;
}

void fetch(uint16_t *memory, int pc, uint16_t *instrucao) {
    *instrucao = memory[pc];
}

void decode(uint16_t instrucao, int *formato, int *opcodeR, int *destino, int *ope1, int *ope2, int *opcodeI, int *registrador, int *imediato) {
    *formato = extract_bits(instrucao, 15, 1);

    switch (*formato) {
        case 0:
            *opcodeR = extract_bits(instrucao, 9, 6);
            *destino = extract_bits(instrucao, 6, 3);
            *ope1 = extract_bits(instrucao, 3, 3);
            *ope2 = extract_bits(instrucao, 0, 3);
            break;

        case 1:
            *opcodeI = extract_bits(instrucao, 13, 2);
            *registrador = extract_bits(instrucao, 10, 3);
            *imediato = extract_bits(instrucao, 0, 10);
            break;
    }
}

void execute(int formato, int opcodeR, int destino, int ope1, int ope2, int opcodeI, int registrador, int imediato, int *pc_increment) {
    switch (formato) {
        case 0:
            printf("Formato: R\n");

            switch (opcodeR) {
                case 0:
                    printf("Add\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", vector[ope1]);
                    printf("Operando 2: %d\n", vector[ope2]);
                    teste1 = vector[ope1];
                    teste2 = vector[ope2];
                    vector[destino] = teste1 + teste2;
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 1:
                    printf("Sub\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    teste1 = vector[ope1];
                    teste2 = vector[ope2];
                    vector[destino] = teste1 - teste2;
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 2:
                    printf("Mul\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    teste1 = vector[ope1];
                    teste2 = vector[ope2];
                    vector[destino] = teste1 * teste2;
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 3:
                    printf("Div\n");                    
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    teste1 = vector[ope1];
                    teste2 = vector[ope2];
                    vector[destino] = teste1 / teste2;
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 4:
                    printf("cmp_equal\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    teste2 = vector[ope2];
                    teste1 = vector[ope1];
                    if (teste1 == teste2) {
                        vector[destino] = 1;
                    } else {
                        vector[destino] = 0;
                    }
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 5:
                    printf("cmp_not_equal\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    teste1 = vector[ope1];
                    teste2 = vector[ope2];
                    if (teste1 != teste2) {
                        vector[destino] = 1;
                    } else {
                        vector[destino] = 0;
                    }
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 6:
                    printf("cmp_less_than\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    teste1 = vector[ope1];
                    teste2 = vector[ope2];
                    if (teste1 < teste2) {
                        vector[destino] = 1;
                    } else {
                        vector[destino] = 0;
                    }
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 7:
                    printf("cmp_greater_than\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    teste1 = vector[ope1];
                    teste2 = vector[ope2];
                    if (teste1 > teste2) {
                        vector[destino] = 1;
                    } else {
                        vector[destino] = 0;
                    }
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 8:
                    printf("cmp_less_than_or_equal\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    teste1 = vector[ope1];
                    teste2 = vector[ope2];
                    if (teste1 <= teste2) {
                        vector[destino] = 1;
                    } else {
                        vector[destino] = 0;
                    }
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 9:
                    printf("cmp_greater_than_or_equal\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    teste1 = vector[ope1];
                    teste2 = vector[ope2];
                    if (teste1 >= teste2) {
                        vector[destino] = 1;
                    } else {
                        vector[destino] = 0;
                    }
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 10:
                    printf("and\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    teste1 = vector[ope1];
                    teste2 = vector[ope2];
                    vector[destino] = teste1 & teste2;
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 11:
                    printf("or\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    teste1 = vector[ope1];
                    teste2 = vector[ope2];
                    vector[destino] = teste1 | teste2;
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 12:
                    printf("Xor\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    teste1 = vector[ope1];
                    teste2 = vector[ope2];
                    vector[destino] = teste1 ^ teste2;
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 13:
                    printf("Shl\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    teste1 = vector[ope1];
                    teste2 = vector[ope2];
                    vector[destino] = teste1 << teste2;
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 14:
                    printf("Shr\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    teste1 = vector[ope1];
                    teste2 = vector[ope2];
                    vector[destino] = teste1 >> teste2;
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 15:
                    printf("Load\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    vector[ope1] = vector[destino];
                    printf("Resultado: %d\n", vector[destino]);
                    break;

                case 16:
                    printf("Store\n");
                    printf("Destino: %d\n", destino);
                    printf("Operando 1: %d\n", ope1);
                    printf("Operando 2: %d\n", ope2);
                    vector[ope1] = vector[ope2];
                    vector[ope1] = vector[ope2];
                    printf("Resultado: %d\n", vector[ope1]);
                    break;
                
                case 63:
                    //isRunning = 0;
                    break;
            }
            break;

        case 1:
            printf("Formato: I\n");

            switch (opcodeI) {
                case 0:
                    printf("Jump\n");
                    printf("Registrador: %d\n", registrador);
                    printf("Imediato: %d\n", imediato);
                    pc = imediato;
                    *pc_increment = 0;
                    printf("Resultado: %d\n", pc);
                    break;

                case 1:
                    printf("Jump_cond\n");
                    printf("Registrador: %d\n", registrador);
                    printf("Imediato: %d\n", imediato);
                    if (vector[registrador] == 1) {
                        pc = imediato;
                        *pc_increment = 0;
                    }
                    printf("Resultado: %d\n", pc);
                    break;

                case 3:
                    printf("Mov\n");
                    printf("Registrador: %d\n", registrador);
                    printf("Imediato: %d\n", imediato);
                    vector[registrador] = imediato;
                    printf("Resultado: %d\n", vector[registrador]);
                    break;
            }
            break;
    }
}