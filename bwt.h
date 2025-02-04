/*
Burrows–Wheeler transform

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#pragma once

#include <stdlib.h>

#define ALPHABET_SIZE 256
#define BLOCK_POWER 21
#define BLOCK_SIZE (1 << BLOCK_POWER)

static int bwt_length;
static const unsigned char *bwt_input;
static unsigned char *bwt_output;

int bwt_compare(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	for (int i = 0; i < bwt_length; ++i) {
		int l = bwt_input[(x + i) % bwt_length];
		int r = bwt_input[(y + i) % bwt_length];
		if (l < r)
			return -1;
		if (l > r)
			return 1;
	}
	return 0;
}

int bwt(unsigned char *output, const unsigned char *input, int length) {
	bwt_output = output;
	bwt_input = input;
	bwt_length = length;
	static int rotations[BLOCK_SIZE];
	for (int i = 0; i < length; ++i)
		rotations[i] = i;
	qsort(rotations, length, sizeof(int), bwt_compare);
	for (int i = 0; i < length; ++i)
		output[i] = input[(rotations[i] + length - 1) % length];
	int row = 0;
	while (rotations[row])
		++row;
	return row;
}

void ibwt(unsigned char *output, const unsigned char *input, int length, int row) {
	static int freq[ALPHABET_SIZE];
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		freq[i] = 0;
	for (int i = 0; i < length; ++i)
		++freq[input[i]];
	static int first[ALPHABET_SIZE];
	first[0] = 0;
	for (int i = 1; i < ALPHABET_SIZE; ++i)
		first[i] = first[i - 1] + freq[i - 1];
	static int count[ALPHABET_SIZE];
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		count[i] = 0;
	static int last[BLOCK_SIZE];
	for (int i = 0; i < length; ++i)
		last[i] = ++count[input[i]];
	static int lfm[BLOCK_SIZE];
	for (int i = 0; i < length; ++i)
		lfm[i] = first[input[i]] + last[i] - 1;
	for (int i = length-1; i >= 0; --i, row = lfm[row])
		output[i] = input[row];
}

