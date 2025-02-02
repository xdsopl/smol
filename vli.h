/*
Variable length integer

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#pragma once

#include "bits.h"

int putval(int val) {
	static int order;
	while (val >= 1 << order) {
		if (putbit(0))
			return -1;
		val -= 1 << order;
		order += 1;
	}
	if (putbit(1))
		return -1;
	if (write_bits(val, order))
		return -1;
	order -= 1;
	if (order < 0)
		order = 0;
	return 0;
}

int getval() {
	static int order;
	int val, sum = 0, ret;
	while ((ret = getbit()) == 0) {
		sum += 1 << order;
		order += 1;
	}
	if (ret < 0)
		return -1;
	if (read_bits(&val, order))
		return -1;
	order -= 1;
	if (order < 0)
		order = 0;
	return val + sum;
}

