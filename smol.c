/*
Compression of English text

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

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
	if (enc) {
		int block_power = 8;
		if (argc == 3)
			block_power = atoi(argv[2]);
		if (block_power < 6 || block_power > POWER)
			return 1;
		if (write_bits(block_power - 6, 4))
			return 1;
		int block_length = 1 << block_power;
		int partial = 0;
		while (!partial && (length = fread(ibuffer, 1, block_length, stdin)) > 0) {
			read_bytes += length;
			if (length < block_length) {
				partial = 1;
				if (putbit(1))
					return 1;
				if (write_bits(length, block_power))
					return 1;
			} else {
				if (putbit(0))
					return 1;
			}
			int row = bwt();
			if (write_bits(row, block_power))
				return 1;
			for (int i = 0; i < length; ++i)
				if (putval(get_value(obuffer[i])))
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
		if (block_power < 6 || block_power > POWER)
			return 1;
		int block_length = 1 << block_power;
		int partial = 0;
		while (!partial) {
			length = block_length;
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
				ibuffer[i] = get_symbol(value);
			}
			ibwt(row);
			if (length != (int)fwrite(obuffer, 1, length, stdout))
				return 1;
			wrote_bytes += length;
		}
	}
	double change = 100.0 * (wrote_bytes - read_bytes) / read_bytes;
	fprintf(stderr, "%s: %s %d to %d bytes %+.2f%%\n", argv[0], enc ? "encoded" : "decoded", read_bytes, wrote_bytes, change);
	return 0;
}
