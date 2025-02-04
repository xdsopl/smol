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

int bwt_compare(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	for (; x < bwt_length && y < bwt_length; ++x, ++y)
		if (bwt_input[x] != bwt_input[y])
			return bwt_input[x] - bwt_input[y];
	return y - x;
}

void bwt_sa(int *output, const unsigned char *input, int length) {
	bwt_input = input;
	bwt_length = length;
	for (int i = 0; i < length; ++i)
		output[i] = i;
	qsort(output, length, sizeof(int), bwt_compare);
}

int bwt(unsigned char *output, const unsigned char *input, int length) {
	for (int i = 0; i < length-1; ++i)
		if (!input[i])
			return 1;
	if (input[length-1])
		return 1;
	static int sa[BLOCK_SIZE];
	bwt_sa(sa, input, length);
	for (int i = 0; i < length; ++i) {
		int index = sa[i];
		if (index == 0)
			index = length;
		output[i] = input[index - 1];
	}
	return 0;
}

int ibwt(unsigned char *output, const unsigned char *input, int length) {
	static int count[ALPHABET_SIZE];
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		count[i] = 0;
	static int pref[BLOCK_SIZE];
	for (int i = 0; i < length; ++i)
		pref[i] = count[input[i]]++;
	for (int i = 0, sum = 0; i < ALPHABET_SIZE; ++i) {
		int tmp = count[i] + sum;
		count[i] = sum;
		sum = tmp;
	}
	int row = -1;
	for (int i = 0; i < length; ++i) {
		if (!input[i]) {
			// there can be only one
			if (row >= 0)
				return 1;
			row = i;
		}
	}
	if (row < 0)
		return 1;
	for (int i = length-1; i >= 0; --i) {
		output[i] = input[row];
		row = pref[row] + count[input[row]];
	}
	return 0;
}

