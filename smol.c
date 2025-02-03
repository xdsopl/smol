/*
Compression of English text

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#include "vli.h"
#include "mtf.h"

#define BUFFER 256

static int length;
static unsigned char buffer[BUFFER];

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
		while ((length = fread(buffer, 1, BUFFER, stdin)) > 0) {
			read_bytes += length;
			for (int i = 0; i < length; ++i)
				if (putval(get_value(buffer[i])))
					return 1;
		}
		if (putval(itable[0]))
			return 1;
		flush_bits();
	} else {
		int value;
		while ((value = getval()) >= 0) {
			int symbol = get_symbol(value);
			if (!symbol)
				break;
			if (putbyte(symbol))
				return 1;
		}
	}
	double change = 100.0 * (wrote_bytes - read_bytes) / read_bytes;
	fprintf(stderr, "%s: %s %d to %d bytes %+.2f%%\n", argv[0], enc ? "encoded" : "decoded", read_bytes, wrote_bytes, change);
	return 0;
}
