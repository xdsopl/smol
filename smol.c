/*
Compression of English text

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "table.h"

int main() {
	unsigned char *p;
	for (p = table; *p; ++p)
		printf("%2x: [%c]\n", *p, *p);
	printf("--\n");
	for (int i = 32; i < 127; ++i)
		if (!strchr((char *)table, i))
			printf("%2x: [%c]\n", *p++ = i, i);
	for (int i = 1; i < 32; ++i)
		*p++ = i;
	for (int i = 127; i < 256; ++i)
		*p++ = i;
	*p++ = 0;
	printf("count = %ld\n", p - table);
	static unsigned char itable[256];
	for (int i = 0; i < 256; ++i)
		itable[table[i]] = i;
	for (int i = 0; i < 256; ++i)
		assert(table[itable[i]] == i);
	printf("static const unsigned char table[256] = { ");
	for (int i = 0; i < 256; ++i)
		printf("%d, ", table[i]);
	printf("};\nstatic const unsigned char itable[256] = { ");
	for (int i = 0; i < 256; ++i)
		printf("%d, ", itable[i]);
	printf("};\n");
	return 0;
}
