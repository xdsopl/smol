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
	for (int i = 0; i < bwt_length; ++i) {
		int l = bwt_input[(x + i) % bwt_length];
		int r = bwt_input[(y + i) % bwt_length];
		if (l != r)
			return l - r;
	}
	return 0;
}

void bwt_rot(int *output, const unsigned char *input, int length) {
	bwt_input = input;
	bwt_length = length;
	for (int i = 0; i < length; ++i)
		output[i] = i;
	qsort(output, length, sizeof(int), bwt_compare);
}

int bwt(unsigned char *output, const unsigned char *input, int length) {
	static int rot[BLOCK_SIZE];
	bwt_rot(rot, input, length);
	for (int i = 0; i < length; ++i)
		output[i] = input[(rot[i] + length - 1) % length];
	int row = 0;
	while (rot[row])
		++row;
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
		sum += count[i];
		count[i] = sum - count[i];
	}
	for (int i = length-1; i >= 0; --i) {
		output[i] = input[row];
		row = pref[row] + count[input[row]];
	}
}

