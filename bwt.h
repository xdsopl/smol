/*
Burrows–Wheeler transform

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#pragma once

#include <stdlib.h>

static int length;
#define BUFFER 4096

static int length;
static unsigned char ibuffer[BUFFER], obuffer[BUFFER];

int compare(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	for (int i = 0; i < length; ++i) {
		int l = ibuffer[(x + i) % length];
		int r = ibuffer[(y + i) % length];
		if (l < r)
			return -1;
		if (l > r)
			return 1;
	}
	return 0;
}

void bwt() {
	static int rotations[BUFFER];
	for (int i = 0; i < length; ++i)
		rotations[i] = i;
	qsort(rotations, length, sizeof(int), compare);
	for (int i = 0; i < length; ++i)
		obuffer[i] = ibuffer[(rotations[i] + length - 1) % length];
}

void ibwt(int end) {
	static int freq[256];
	for (int i = 0; i < 256; ++i)
		freq[i] = 0;
	for (int i = 0; i < length; ++i)
		++freq[ibuffer[i]];
	static int first[256];
	for (int i = 1; i < 256; ++i)
		first[i] = first[i - 1] + freq[i - 1];
	static int count[256];
	for (int i = 0; i < 256; ++i)
		count[i] = 0;
	static int last[BUFFER];
	for (int i = 0; i < length; ++i)
		last[i] = ++count[ibuffer[i]];
	static int lfm[BUFFER];
	for (int i = 0; i < length; ++i)
		lfm[i] = first[ibuffer[i]] + last[i] - 1;
	int row = 0;
	while (row < length && ibuffer[row] != end)
		++row;
	for (int i = length-1; i >= 0; --i, row = lfm[row])
		obuffer[i] = ibuffer[row];
}

