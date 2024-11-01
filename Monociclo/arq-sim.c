#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "lib.h"
#define TAMANHO_DE_MEMORIA 0x0100

int estaRodando = 1;
int vetorDeReg[8] = {0};
int pc = 0;
int vaiPular = 1;

typedef struct Instrucao{
    uint16_t formato;
    uint16_t opcodeR;
    uint16_t destino;
    uint16_t ope1;
    uint16_t ope2;
    uint16_t opcodeI;
    uint16_t registrador;
    uint16_t imediato;
} Instrucao;

void print_binario(uint16_t value){
    for (int i = 15; i >= 0; i--){
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

void busca(uint16_t *memoria, int pc, uint16_t *instrucao){
    *instrucao = memoria[pc];
}

Instrucao decodificacao(uint16_t instrucao){
    Instrucao instrucoes;
    instrucoes.formato = extract_bits(instrucao, 15, 1); 
    if(instrucoes.formato == 0){
        instrucoes.opcodeR = extract_bits(instrucao, 9, 6);
        instrucoes.destino = extract_bits(instrucao, 6, 3);
        instrucoes.ope1 = extract_bits(instrucao, 3, 3);
        instrucoes.ope2 = extract_bits(instrucao, 0, 3);
    } else if (instrucoes.formato == 1){
        instrucoes.opcodeI = extract_bits(instrucao, 13, 2);
        instrucoes.registrador = extract_bits(instrucao, 10, 3);
        instrucoes.imediato = extract_bits(instrucao, 0, 10);
    }
    return instrucoes;
}

void execucao(uint16_t *memoria, Instrucao conjuntoInstrucao){
    vaiPular = 1;
    
    switch (conjuntoInstrucao.formato){
        case 0:
            switch (conjuntoInstrucao.opcodeR){
                case 0:
                    printf("ADD\n");
                    vetorDeReg[conjuntoInstrucao.destino] = vetorDeReg[conjuntoInstrucao.ope1] + vetorDeReg[conjuntoInstrucao.ope2];
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 1:
                    printf("SUB\n");
                    vetorDeReg[conjuntoInstrucao.destino] = vetorDeReg[conjuntoInstrucao.ope1] - vetorDeReg[conjuntoInstrucao.ope2];
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 2:
                    printf("MUL\n");
                    vetorDeReg[conjuntoInstrucao.destino] = vetorDeReg[conjuntoInstrucao.ope1] * vetorDeReg[conjuntoInstrucao.ope2];
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 3:
                    printf("DIV\n");
                    vetorDeReg[conjuntoInstrucao.destino] = vetorDeReg[conjuntoInstrucao.ope1] / vetorDeReg[conjuntoInstrucao.ope2];
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 4:
                    printf("CMP_EQUAL\n");
                    if(vetorDeReg[conjuntoInstrucao.ope1] == vetorDeReg[conjuntoInstrucao.ope2]){
                        vetorDeReg[conjuntoInstrucao.destino] = 1;
                    } else {
                        vetorDeReg[conjuntoInstrucao.destino] = 0;
                    }
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 5:
                    printf("CMP_NEQ\n");
                    if(vetorDeReg[conjuntoInstrucao.ope1] != vetorDeReg[conjuntoInstrucao.ope2]){
                        vetorDeReg[conjuntoInstrucao.destino] = 1;
                    } else {
                        vetorDeReg[conjuntoInstrucao.destino] = 0;
                    }
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 6:
                    printf("CMP_LESS\n");
                    if(vetorDeReg[conjuntoInstrucao.ope1] < vetorDeReg[conjuntoInstrucao.ope2]){
                        vetorDeReg[conjuntoInstrucao.destino] = 1;
                    } else {
                        vetorDeReg[conjuntoInstrucao.destino] = 0;
                    }
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 7:
                    printf("CMP_GREATER\n");
                    if(vetorDeReg[conjuntoInstrucao.ope1] > vetorDeReg[conjuntoInstrucao.ope2]){
                        vetorDeReg[conjuntoInstrucao.destino] = 1;
                    } else {
                        vetorDeReg[conjuntoInstrucao.destino] = 0;
                    }
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 8:
                    printf("CMP_LESS_EQ\n");
                    if(vetorDeReg[conjuntoInstrucao.ope1] <= vetorDeReg[conjuntoInstrucao.ope2]){
                        vetorDeReg[conjuntoInstrucao.destino] = 1;
                    } else {
                        vetorDeReg[conjuntoInstrucao.destino] = 0;
                    }
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 9:
                    printf("CMP_GREATER_EQ\n");
                    if(vetorDeReg[conjuntoInstrucao.ope1] >= vetorDeReg[conjuntoInstrucao.ope2]){
                        vetorDeReg[conjuntoInstrucao.destino] = 1;
                    } else {
                        vetorDeReg[conjuntoInstrucao.destino] = 0;
                    }
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 10:
                    printf("AND\n");
                    vetorDeReg[conjuntoInstrucao.destino] = vetorDeReg[conjuntoInstrucao.ope1] && vetorDeReg[conjuntoInstrucao.ope2];
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 11:
                    printf("OR\n");
                    vetorDeReg[conjuntoInstrucao.destino] = vetorDeReg[conjuntoInstrucao.ope1] || vetorDeReg[conjuntoInstrucao.ope2];
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 12:
                    printf("XOR\n");
                    vetorDeReg[conjuntoInstrucao.destino] = vetorDeReg[conjuntoInstrucao.ope1] ^ vetorDeReg[conjuntoInstrucao.ope2];
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 13:
                    printf("SHL\n");
                    vetorDeReg[conjuntoInstrucao.destino] = vetorDeReg[conjuntoInstrucao.ope1] << vetorDeReg[conjuntoInstrucao.ope2];
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 14:
                    printf("SHR\n");
                    vetorDeReg[conjuntoInstrucao.destino] = vetorDeReg[conjuntoInstrucao.ope1] >> vetorDeReg[conjuntoInstrucao.ope2];
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 15:
                    printf("LOAD\n");
                    int endLoadRegOpe1 = vetorDeReg[conjuntoInstrucao.ope1];
                    vetorDeReg[conjuntoInstrucao.destino] = memoria[endLoadRegOpe1]; 
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.destino, vetorDeReg[conjuntoInstrucao.destino]);
                    break;
                case 16:
                    printf("STORE\n");
                    int endStoreRegOpe1 = vetorDeReg[conjuntoInstrucao.ope1];
                    memoria[endStoreRegOpe1] = vetorDeReg[conjuntoInstrucao.ope2];
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.ope1, vetorDeReg[conjuntoInstrucao.ope2]);
                    break;
                case 63:
                    printf("SYSCALL\n");
                    if(vetorDeReg[0] == 0){
                        estaRodando = 0;
                    }
                    break;
            }
            break;
        case 1:
            switch (conjuntoInstrucao.opcodeI){
                case 0:
                    printf("JUMP\n");
                    pc = conjuntoInstrucao.imediato;
                    vaiPular = 0;
                    printf("PULOUUU\n");
                    break;
                case 1:
                    printf("JUMP_COND\n");
                    if(vetorDeReg[conjuntoInstrucao.registrador] == 1){
                        pc = conjuntoInstrucao.imediato;
                        vaiPular = 0;
                        printf("PULOUUU\n");
                    }
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.registrador, vetorDeReg[conjuntoInstrucao.registrador]);
                    break;
                case 3:
                    printf("MOV\n");
                    vetorDeReg[conjuntoInstrucao.registrador] = conjuntoInstrucao.imediato;
                    printf("Resultado%d = %d\n ", conjuntoInstrucao.registrador, vetorDeReg[conjuntoInstrucao.registrador]);
                    break;
            }
            break;
    }
}

int main (int argc, char **argv){
	if(argc != 2){
		printf("usage: %s [bin_name]\n", argv[0]);
		exit(1);
	}

    uint16_t memoria[TAMANHO_DE_MEMORIA] = {0};
    load_binary_to_memory(argv[1], memoria, TAMANHO_DE_MEMORIA);

    while(estaRodando){
        uint16_t instrucao;
        busca(memoria, pc, &instrucao);

        Instrucao conjuntoInstrucao = decodificacao(instrucao);
        printf("--------------------\n");
        printf("%d\n", pc);
        print_binario(instrucao);
        printf("Formato: %d\n", conjuntoInstrucao.formato);
        if(conjuntoInstrucao.formato == 0){
            printf("Opcode: %d\n", conjuntoInstrucao.opcodeR);
            printf("Destino: %d\n", conjuntoInstrucao.destino);
            printf("Operando 1: %d\n", conjuntoInstrucao.ope1);
            printf("Operando 2: %d\n", conjuntoInstrucao.ope2);
        } else if (conjuntoInstrucao.formato == 1){
            printf("Opcode: %d\n", conjuntoInstrucao.opcodeI);
            printf("Registrador: %d\n", conjuntoInstrucao.registrador);
            printf("Imediato: %d\n", conjuntoInstrucao.imediato);
        }

        execucao(memoria, conjuntoInstrucao);

        if(vaiPular){
            pc++;
        }
        if(pc >= TAMANHO_DE_MEMORIA){
            estaRodando = 0;
        }
    }

    printf("----------------------\n");
    for(int i = 0; i < 8; i++){
        printf("Registrador %d: %d\n", i, vetorDeReg[i]);
    }
    printf("----------------------\n");
    for(int i = 0; i <= 100; i++){
        printf("%d ", memoria[i]);
    }
    printf("\n");
	return 0;
}
