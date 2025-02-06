/*
Burrows–Wheeler transform

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#pragma once

#include <stdlib.h>

#define ALPHABET_SIZE 256
#define BLOCK_POWER 21
#define BLOCK_SIZE (1 << BLOCK_POWER)

static int rank[2*BLOCK_SIZE], next[2*BLOCK_SIZE];

int bwt_compare(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	if (rank[x] != rank[y])
		return rank[x] - rank[y];
	return next[x] - next[y];
}

void bwt_sa(int *sa, const unsigned char *input, int length) {
	for (int i = 0; i < length; ++i) {
		sa[i] = i;
		rank[i] = input[i];
		next[i] = (i + 1 < length) ? input[i + 1] : -1;
	}
	qsort(sa, length, sizeof(int), bwt_compare);
	for (int k = 2; k < length; k *= 2) {
		static int temp[2*BLOCK_SIZE];
		int current = 0;
		temp[sa[0]] = 0;
		for (int i = 1; i < length; ++i) {
			if (rank[sa[i]] != rank[sa[i - 1]] || next[sa[i]] != next[sa[i - 1]])
				++current;
			temp[sa[i]] = current;
		}
		for (int i = 0; i < length; ++i) {
			rank[sa[i]] = temp[sa[i]];
			next[sa[i]] = (sa[i] + k < length) ? temp[sa[i] + k] : -1;
		}
		qsort(sa, length, sizeof(int), bwt_compare);
		if (current == length - 1)
			break;
	}
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

