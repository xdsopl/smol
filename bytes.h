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

int read_bytes(int *buffer, int bytes) {
	int count = 0;
	while (bytes--) {
		int byte = getbyte();
		if (byte < 0)
			return count;
		buffer[count++] = byte;
	}
	return count;
}

int write_bytes(const int *buffer, int bytes) {
	while (bytes--)
		if (putbyte(*buffer++))
			return 1;
	return 0;
}

