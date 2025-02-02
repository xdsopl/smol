/*
Compression of English text

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "table.h"

int main(int argc, char **argv) {
	if (argc != 2)
		return 1;
	if (*argv[1] != 'e' && *argv[1] != 'd')
		return 1;
	const unsigned char *perm = *argv[1] == 'e' ? itable : table;
	int byte;
	while ((byte = getchar()) != EOF)
		if (putchar(perm[byte]) == EOF)
			return 1;
	return 0;
}
