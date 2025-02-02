/*
Variable length integer

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#pragma once

#include "bits.h"

int putval(int val) {
	int cnt = 0, top = 1;
	while (top <= val) {
		cnt += 1;
		top = 1 << cnt;
		if (putbit(1))
			return -1;
	}
	if (putbit(0))
		return -1;
	if (cnt > 0) {
		cnt -= 1;
		val -= top / 2;
		if (write_bits(val, cnt))
			return -1;
	}
	return 0;
}

int getval() {
	int val = 0, cnt = 0, top = 1, ret;
	while ((ret = getbit()) == 1) {
		cnt += 1;
		top = 1 << cnt;
	}
	if (ret < 0)
		return -1;
	if (cnt > 0) {
		cnt -= 1;
		if (read_bits(&val, cnt))
			return -1;
		val += top / 2;
	}
	return val;
}

