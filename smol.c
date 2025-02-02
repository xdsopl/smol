/*
Compression of English text

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#include "vli.h"
#include "table.h"

void move_to_front(int byte) {
	int index = itable[byte];
	for (int i = index; i; --i)
		table[i] = table[i - 1];
	table[0] = byte;
	for (int i = 0; i < 256; ++i)
		itable[table[i]] = i;
}

int main(int argc, char **argv) {
	if (argc != 2)
		return 1;
	if (*argv[1] != 'e' && *argv[1] != 'd')
		return 1;
	int enc = *argv[1] == 'e';
	if (enc) {
		int byte;
		while ((byte = getbyte()) >= 0) {
			if (putval(itable[byte]))
				return 1;
			move_to_front(byte);
		}
		if (putval(itable[0]))
			return 1;
		flush_bits();
	} else {
		int value;
		while ((value = getval()) >= 0) {
			int byte = table[value];
			if (!byte)
				break;
			move_to_front(byte);
			if (putbyte(byte))
				return 1;
		}
	}
	double change = 100.0 * (wrote_bytes - read_bytes) / read_bytes;
	fprintf(stderr, "%s: %s %d to %d bytes %+.2f%%\n", argv[0], enc ? "encoded" : "decoded", read_bytes, wrote_bytes, change);
	return 0;
}
