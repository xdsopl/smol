/*
Run length encoding

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#pragma once

#include "vli.h"

int putrle(int value) {
	static int prev = -1, count;
	if (prev == value) {
		++count;
	} else if (prev >= 0) {
		if (putval(prev))
			return -1;
		if (putval(count))
			return -1;
		count = 0;
	}
	prev = value;
	return 0;
}

int getrle() {
	static int value, count;
	if (count) {
		--count;
	} else {
		value = getval();
		if (value < 0)
			return -1;
		count = getval();
		if (count < 0)
			return -1;
	}
	return value;
}

