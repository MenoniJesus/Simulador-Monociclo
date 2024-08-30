#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "lib.h"
#define MEMORY_SIZE 0x0100

int vetorDeReg[8] = {0};

//Serve só para printar bonitinho o binario
void print_binary(uint16_t value) {
    for (int i = 15; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

void busca(uint16_t *memory, int pc, uint16_t *instrucao){
    *instrucao = memory[pc];
}

void decodificacao(uint16_t instrucao, int *formato, int *opcodeR, int *destino, int *ope1, int *ope2, int *opcodeI, int *registrador, int *imediato){
    *formato = extract_bits(instrucao, 15, 1); 
    if (*formato == 0){
        *opcodeR = extract_bits(instrucao, 9, 6);
        *destino = extract_bits(instrucao, 6, 3);
        *ope1 = extract_bits(instrucao, 3, 3);
        *ope2 = extract_bits(instrucao, 0, 3);
    } else if (*formato == 1){
        *opcodeI = extract_bits(instrucao, 13, 2);
        *registrador = extract_bits(instrucao, 10, 3);
        *imediato = extract_bits(instrucao, 0, 10);
    }
}

void execucao(int formato, int opcodeR, int destino, int ope1, int ope2, int opcodeI, int registrador, int imediato, int *pc, int *isJumping){
    switch (formato){
        case 0:
            switch (opcodeR){
                case 0:
                    printf("ADD\n");
                    break;
                case 1:
                    printf("SUB\n");
                    break;
                case 2:
                    printf("MUL\n");
                    break;
                case 3:
                    printf("DIV\n");
                    break;
                case 4:
                    printf("CMP_EQUAL\n");
                    break;
                case 5:
                    printf("CMP_NEQ\n");
                    break;
                case 6:
                    printf("CMP_LESS\n");
                    break;
                case 7:
                    printf("CMP_GREATER\n");
                    break;
                case 8:
                    printf("CMP_LESS_EQ\n");
                    break;
                case 9:
                    printf("CMP_GREATER_EQ\n");
                    break;
                case 10:
                    printf("AND\n");
                    break;
                case 11:
                    printf("OR\n");
                    break;
                case 12:
                    printf("XOR\n");
                    break;
                case 13:
                    printf("SHL\n");
                    break;
                case 14:
                    printf("SHR\n");
                    break;
                case 15:
                    printf("LOAD\n");
                    break;
                case 16:
                    printf("STORE\n");
                    break;
                case 63:
                    printf("SYSCALL\n");
                    break;
            }
            break;
        
        case 1:
            switch (opcodeI){
                case 0:
                    printf("JUMP\n");
                    break;
                case 1:
                    printf("JUMP_COND\n");
                    break;
                case 2:
                    printf("NÃO USADO\n");
                    break;
                case 3:
                    printf("MOV\n");
                    break;
            }
            break;
    }

}

int main (int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: %s [bin_name]\n", argv[0]);
		exit(1);
	}

    uint16_t memory[MEMORY_SIZE] = {0};
    load_binary_to_memory(argv[1], memory, MEMORY_SIZE);

    int isRunning = 1;
    int pc = 0;

    while(isRunning){
        uint16_t instrucao;
        busca(memory, pc, &instrucao);

        int formato, opcodeR, destino, ope1, ope2, opcodeI, registrador, imediato;
        decodificacao(instrucao, &formato, &opcodeR, &destino, &ope1, &ope2, &opcodeI, &registrador, &imediato);
        printf("--------------------\n");
        printf("%d\n", pc);
        print_binary(instrucao);
        printf("Formato: %d\n", formato);
        if (formato == 0){
            printf("Opcode: %d\n", opcodeR);
            printf("Destino: %d\n", destino);
            printf("Operando 1: %d\n", ope1);
            printf("Operando 2: %d\n", ope2);
        } else if (formato == 1){
            printf("Opcode: %d\n", opcodeI);
            printf("Registrador: %d\n", registrador);
            printf("Imediato: %d\n", imediato);
        }

        int isJumping = 1;

        execucao(formato, opcodeR, destino, ope1, ope2, opcodeI, registrador, imediato, &pc, &isJumping);

        if(isJumping){
            pc++;
        }

        if(pc >= MEMORY_SIZE){
            isRunning = 0;
        }
    }

	return 0;
}