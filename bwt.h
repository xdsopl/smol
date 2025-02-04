/*
Burrows–Wheeler transform

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#pragma once

#include <stdlib.h>

#define ALPHABET_SIZE 256
#define BLOCK_POWER 21
#define BLOCK_LENGTH (1 << BLOCK_POWER)

static int length;
static unsigned char iblock[BLOCK_LENGTH], oblock[BLOCK_LENGTH];

int compare(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	for (int i = 0; i < length; ++i) {
		int l = iblock[(x + i) % length];
		int r = iblock[(y + i) % length];
		if (l < r)
			return -1;
		if (l > r)
			return 1;
	}
	return 0;
}

int bwt() {
	static int rotations[BLOCK_LENGTH];
	for (int i = 0; i < length; ++i)
		rotations[i] = i;
	qsort(rotations, length, sizeof(int), compare);
	for (int i = 0; i < length; ++i)
		oblock[i] = iblock[(rotations[i] + length - 1) % length];
	int row = 0;
	while (rotations[row])
		++row;
	return row;
}

void ibwt(int row) {
	static int freq[ALPHABET_SIZE];
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		freq[i] = 0;
	for (int i = 0; i < length; ++i)
		++freq[iblock[i]];
	static int first[ALPHABET_SIZE];
	for (int i = 1; i < ALPHABET_SIZE; ++i)
		first[i] = first[i - 1] + freq[i - 1];
	static int count[ALPHABET_SIZE];
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		count[i] = 0;
	static int last[BLOCK_LENGTH];
	for (int i = 0; i < length; ++i)
		last[i] = ++count[iblock[i]];
	static int lfm[BLOCK_LENGTH];
	for (int i = 0; i < length; ++i)
		lfm[i] = first[iblock[i]] + last[i] - 1;
	for (int i = length-1; i >= 0; --i, row = lfm[row])
		oblock[i] = iblock[row];
}

