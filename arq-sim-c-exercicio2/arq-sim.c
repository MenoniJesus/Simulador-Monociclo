#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "lib.h"
#define MEMORY_SIZE 0x0100

void print_binary(uint16_t value) {
    for (int i = 15; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

int main (int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: %s [bin_name]\n", argv[0]);
		exit(1);
	}

    uint16_t memory[MEMORY_SIZE] = {0};
    load_binary_to_memory(argv[1], memory, MEMORY_SIZE);

    for(int i = 0; i < MEMORY_SIZE; i++) {
        printf("memory[%d] = ", i);
        print_binary(memory[i]);
    }

	return 0;
}