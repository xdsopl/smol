/*
Variable length integer

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#pragma once

#include "bits.h"

int putval(int val, int ctx) {
	static int order[2];
	while (val >= 1 << order[ctx]) {
		if (putbit(0))
			return -1;
		val -= 1 << order[ctx];
		order[ctx] += 1;
	}
	if (putbit(1))
		return -1;
	if (write_bits(val, order[ctx]))
		return -1;
	order[ctx] -= 1 + ctx;
	if (order[ctx] < 0)
		order[ctx] = 0;
	return 0;
}

int getval(int ctx) {
	static int order[2];
	int val, sum = 0, ret;
	while ((ret = getbit()) == 0) {
		sum += 1 << order[ctx];
		order[ctx] += 1;
	}
	if (ret < 0)
		return -1;
	if (read_bits(&val, order[ctx]))
		return -1;
	order[ctx] -= 1 + ctx;
	if (order[ctx] < 0)
		order[ctx] = 0;
	return val + sum;
}

