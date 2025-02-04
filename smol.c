/*
Compression of English text

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include "vli.h"
#include "mtf.h"
#include "bwt.h"

int main(int argc, char **argv) {
	if (argc < 2 || argc > 3)
		return 1;
	if (*argv[1] == 't') {
		print_table();
		return 0;
	}
	if (*argv[1] != 'e' && *argv[1] != 'd')
		return 1;
	int enc = *argv[1] == 'e';
	if (!enc && argc != 2)
		return 1;
	init_mtf();
	static unsigned char input[BLOCK_SIZE], output[BLOCK_SIZE];
	if (enc) {
		int block_power = 8;
		if (argc == 3)
			block_power = atoi(argv[2]);
		if (block_power < 6 || block_power > BLOCK_POWER)
			return 1;
		if (write_bits(block_power - 6, 4))
			return 1;
		int block_size = 1 << block_power;
		int partial = 0;
		int length;
		while (!partial && (length = read_bytes(input, block_size)) > 0) {
			if (length < block_size) {
				partial = 1;
				if (putbit(1))
					return 1;
				if (write_bits(length, block_power))
					return 1;
			} else {
				if (putbit(0))
					return 1;
			}
			int row = bwt(output, input, length);
			if (write_bits(row, block_power))
				return 1;
			for (int i = 0; i < length; ++i)
				if (putval(get_value(output[i])))
					return 1;
		}
		if (!partial) {
			if (putbit(1))
				return 1;
			if (write_bits(0, block_power))
				return 1;
		}
		if (flush_bits())
			return 1;
	} else {
		int block_power;
		if (read_bits(&block_power, 4))
			return 1;
		block_power += 6;
		if (block_power < 6 || block_power > BLOCK_POWER)
			return 1;
		int block_size = 1 << block_power;
		int partial = 0;
		while (!partial) {
			int length = block_size;
			partial = getbit();
			if (partial < 0)
				return 1;
			if (partial && read_bits(&length, block_power))
				return 1;
			if (!length)
				break;
			int row;
			if (read_bits(&row, block_power))
				return 1;
			for (int i = 0; i < length; ++i) {
				int value = getval();
				if (value < 0)
					return 1;
				input[i] = get_symbol(value);
			}
			ibwt(output, input, length, row);
			if (write_bytes(output, length))
				return 1;
		}
	}
	double change = 100.0 * (bytes_written - bytes_read) / bytes_read;
	fprintf(stderr, "%s: %s %d to %d bytes %+.2f%%\n", argv[0], enc ? "encoded" : "decoded", bytes_read, bytes_written, change);
	return 0;
}
