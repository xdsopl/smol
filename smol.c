/*
Compression of English text

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#include <stdio.h>
#include "table.h"

int main() {
	for (unsigned char *p = table; *p; ++p)
		printf("%2x: [%c]\n", *p, *p);
	return 0;
}
