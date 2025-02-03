/*
Compression of English text

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#include "vli.h"
#include "mtf.h"
#include "bwt.h"

#define ETX 3
#define EOT 4

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
		while ((length = fread(ibuffer, 1, BUFFER-1, stdin)) > 0) {
			read_bytes += length;
			ibuffer[length++] = ETX;
			bwt();
			for (int i = 0; i < length; ++i)
				if (putval(get_value(obuffer[i])))
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
				ibuffer[length] = symbol;
			}
			if (!length)
				break;
			ibwt(ETX);
			if (length-1 != (int)fwrite(obuffer, 1, length-1, stdout))
				return 1;
			wrote_bytes += length-1;
		} while (length == BUFFER);
	}
	double change = 100.0 * (wrote_bytes - read_bytes) / read_bytes;
	fprintf(stderr, "%s: %s %d to %d bytes %+.2f%%\n", argv[0], enc ? "encoded" : "decoded", read_bytes, wrote_bytes, change);
	return 0;
}
