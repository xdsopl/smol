/*
Reading and writing bits

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#pragma once

#include "bytes.h"

int getbit() {
	static int acc, cnt;
	if (!cnt) {
		int byte = getbyte();
		if (byte < 0)
			return -1;
		acc = byte;
		cnt = 8;
	}
	int bit = acc & 1;
	acc >>= 1;
	cnt -= 1;
	return bit;
}

int putbit(int bit) {
	static int acc, cnt;
	if (bit < 0) {
		if (cnt) {
			if (putbyte(acc))
				return -1;
			cnt = 0;
			acc = 0;
		}
		return 0;
	}
	acc |= !!bit << cnt++;
	if (cnt >= 8) {
		cnt -= 8;
		int byte = acc;
		acc >>= 8;
		if (putbyte(byte))
			return -1;
	}
	return 0;
}

int flush_bits() {
	return putbit(-1);
}

int write_bits(int bits, int num) {
	for (int i = 0; i < num; ++i)
		if (putbit((bits >> i) & 1))
			return -1;
	return 0;
}

int read_bits(int *bits, int num) {
	int acc = 0;
	for (int i = 0; i < num; ++i) {
		int bit = getbit();
		if (bit < 0)
			return -1;
		acc |= bit << i;
	}
	*bits = acc;
	return 0;
}

