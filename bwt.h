/*
Burrows–Wheeler transform

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#pragma once

#include <stdlib.h>

#define ALPHABET_SIZE 257
#define BLOCK_POWER 21
#define BLOCK_SIZE (1 << BLOCK_POWER)

static int bwt_length;
static const int *bwt_input;

int bwt_compare(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	for (; x < bwt_length && y < bwt_length; ++x, ++y)
		if (bwt_input[x] != bwt_input[y])
			return bwt_input[x] - bwt_input[y];
	return y - x;
}

void bwt_sa(int *output, const int *input, int length) {
	bwt_input = input;
	bwt_length = length;
	for (int i = 0; i < length; ++i)
		output[i] = i;
	qsort(output, length, sizeof(int), bwt_compare);
}

int bwt(int *output, const int *input, int length) {
	static int row, sa[BLOCK_SIZE];
	bwt_sa(sa, input, length);
	for (int i = 0; i < length; ++i) {
		int index = sa[i];
		if (index == 0) {
			index = length;
			row = i;
		}
		output[i] = input[index - 1];
	}
	return row;
}

void ibwt(int *output, const int *input, int length, int row) {
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

