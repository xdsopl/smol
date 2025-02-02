/*
Compression of English text

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "table.h"

int main() {
	for (int i = 0; i < 256; ++i)
		assert(table[itable[i]] == i);
	return 0;
}
