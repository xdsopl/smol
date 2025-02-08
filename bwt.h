/*
Burrows–Wheeler transform

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#pragma once

#define ALPHABET_SIZE 256
#define BLOCK_POWER 21
#define BLOCK_SIZE (1 << BLOCK_POWER)

void bwt_rot(int *output, const unsigned char *input, int length) {
	for (int i = 0; i < length; ++i)
		output[i] = i;
	for (int j = length - 1; j >= 0; --j) {
		static int count[ALPHABET_SIZE];
		for (int i = 0; i < ALPHABET_SIZE; ++i)
			count[i] = 0;
		for (int i = 0; i < length; ++i)
			++count[input[(output[i] + j) % length]];
		for (int i = 1; i < ALPHABET_SIZE; ++i)
			count[i] += count[i - 1];
		static int temp[BLOCK_SIZE];
		for (int i = length - 1; i >= 0; --i)
			temp[--count[input[(output[i] + j) % length]]] = output[i];
		for (int i = 0; i < length; ++i)
			output[i] = temp[i];
	}
}

int bwt(unsigned char *output, const unsigned char *input, int length) {
	static int row, rot[BLOCK_SIZE];
	bwt_rot(rot, input, length);
	for (int i = 0; i < length; ++i) {
		int index = rot[i];
		if (index == 0) {
			index = length;
			row = i;
		}
		output[i] = input[index - 1];
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

