/*
Compression of English text

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#include <stdlib.h>
#include "vli.h"
#include "mtf.h"

#define BUFFER 256
#define ETX 3
#define EOT 4

static int length;
static unsigned char buffer[BUFFER];

int compare(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	for (int i = 0; i < length; ++i) {
		int l = buffer[(x + i) % length];
		int r = buffer[(y + i) % length];
		if (l < r)
			return -1;
		if (l > r)
			return 1;
	}
	return 0;
}

int main(int argc, char **argv) {
	if (argc != 2)
		return 1;
	if (*argv[1] == 't') {
		print_table();
		return 0;
	}
	if (*argv[1] != 'e' && *argv[1] != 'd')
		return 1;
	int enc = *argv[1] == 'e';
	init_mtf();
	if (enc) {
		while ((length = fread(buffer, 1, BUFFER-1, stdin)) > 0) {
			read_bytes += length;
			buffer[length] = ETX;
			static int rotations[BUFFER];
			for (int i = 0; i < length; ++i)
				rotations[i] = i;
			qsort(rotations, length, sizeof(int), compare);
			for (int i = 0; i < length; ++i)
				if (putval(get_value(buffer[(rotations[i] + length - 1) % length])))
					return 1;
		}
		if (putval(get_value(EOT)))
			return 1;
		flush_bits();
	} else {
		do {
			for (length = 0; length < BUFFER; ++length) {
				int value = getval();
				if (value < 0)
					return 1;
				int symbol = get_symbol(value);
				if (symbol == EOT)
					break;
				buffer[length] = symbol;
			}
			if (!length)
				break;
			for (int i = 0; i < length; ++i) {
				int symbol = buffer[i];
				if (symbol < 32 || symbol >= 127)
					symbol = '?';
				fputc(symbol, stderr);
			}
			fputc('\n', stderr);
		} while (length == BUFFER);
	}
	double change = 100.0 * (wrote_bytes - read_bytes) / read_bytes;
	fprintf(stderr, "%s: %s %d to %d bytes %+.2f%%\n", argv[0], enc ? "encoded" : "decoded", read_bytes, wrote_bytes, change);
	return 0;
}
