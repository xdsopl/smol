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

int bwt(unsigned char *output, unsigned char *input, int length) {
	static int row, sa[2*BLOCK_SIZE];
	for (int i = 0, j = length; i < length; ++i, ++j)
		input[j] = input[i];
	bwt_sa(sa, input, 2*length);
	for (int i = 0, j = 0; i < 2*length; ++i) {
		int index = sa[i];
		if (index >= length)
			continue;
		if (index == 0) {
			index = length;
			row = j;
		}
		output[j++] = input[index - 1];
	}
	return row;
}

void ibwt(unsigned char *output, const unsigned char *input, int length, int row) {
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
	for (int i = length-1; i >= 0; --i) {
		output[i] = input[row];
		row = pref[row] + count[input[row]];
	}
}

