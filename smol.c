/*
Compression of English text

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#include "vli.h"
#include "mtf.h"
#include "bwt.h"

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
		int partial = 0;
		while (!partial && (length = fread(ibuffer, 1, BUFFER, stdin)) > 0) {
			read_bytes += length;
			if (length < BUFFER) {
				partial = 1;
				putbit(1);
				write_bits(length, POWER);
			} else {
				putbit(0);
			}
			int row = bwt();
			write_bits(row, POWER);
			for (int i = 0; i < length; ++i)
				if (putval(get_value(obuffer[i])))
					return 1;
		}
		if (!partial) {
			putbit(1);
			write_bits(0, POWER);
		}
		flush_bits();
	} else {
		int partial = 0;
		while (!partial) {
			length = BUFFER;
			partial = getbit();
			if (partial < 0)
				return 1;
			if (partial && read_bits(&length, POWER))
				return 1;
			if (!length)
				break;
			int row;
			if (read_bits(&row, POWER))
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
