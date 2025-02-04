/*
Reading and writing bytes

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#pragma once

#include <stdio.h>

static int bytes_read;
static int bytes_written;

int getbyte() {
	int byte = getchar();
	if (byte == EOF) {
		fprintf(stderr, "reached end of stream\n");
		return -1;
	}
	++bytes_read;
	return byte;
}

int putbyte(int byte) {
	if (EOF == putchar(byte & 255)) {
		fprintf(stderr, "could not write to stream\n");
		return -1;
	}
	++bytes_written;
	return 0;
}

int read_bytes(unsigned char *buffer, int bytes) {
	bytes = fread(buffer, 1, bytes, stdin);
	bytes_read += bytes;
	return bytes;
}

int write_bytes(unsigned char *buffer, int bytes) {
	if (bytes != (int)fwrite(buffer, 1, bytes, stdout))
		return -1;
	bytes_written += bytes;
	return 0;
}

