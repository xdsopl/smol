/*
Compression of English text

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#include <stdio.h>
#include <string.h>
#include "table.h"

int main() {
	unsigned char *p;
	for (p = table; *p; ++p)
		printf("%2x: [%c]\n", *p, *p);
	printf("--\n");
	for (int i = 32; i < 127; ++i)
		if (!strchr((char *)table, i))
			printf("%2x: [%c]\n", *p++ = i, i);
	for (int i = 127; i < 256; ++i)
		*p++ = i;
	for (int i = 0; i < 32; ++i)
		*p++ = i;
	printf("count = %ld\n", p - table);
	return 0;
}
