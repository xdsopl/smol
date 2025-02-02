/*
Reading and writing bytes

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#pragma once

#include <stdio.h>

static int read_bytes;
static int wrote_bytes;

int getbyte() {
	int byte = getchar();
	if (byte == EOF) {
		fprintf(stderr, "reached end of stream\n");
		return -1;
	}
	++read_bytes;
	return byte;
}

int putbyte(int byte) {
	if (EOF == putchar(byte & 255)) {
		fprintf(stderr, "could not write to stream\n");
		return -1;
	}
	++wrote_bytes;
	return 0;
}

