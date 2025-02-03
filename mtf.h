/*
Move-to-front coding

Copyright 2025 Ahmet Inan <xdsopl@gmail.com>
*/

#pragma once

static unsigned char itable[256], table[256] = { ' ', 'e', 't', 'a', 'o', 'i', 'n', 's', 'r', 'h', 'l', 'd', 'c', 'u', 'm', 'w', 'f', 'p', 'g', 'y', 'b', 'v', 'k', 'x', 'j', 'q', 'z', '\'', ',', '.', '"', '?', '!', ';', ':', '-', 'E', 'T', 'A', 'O', 'I', 'N', 'S', 'R', 'H', 'L', 'D', 'C', 'U', 'M', 'W', 'F', 'P', 'G', 'Y', 'B', 'V', 'K', 'X', 'J', 'Q', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '#', '$', '%', '&', '(', ')', '*', '+', '/', '<', '=', '>', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~', 1, 2, 3, 4, 5, 6, 7, 8, '\t', '\n', 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 0 };

void init_mtf() {
	for (int i = 0; i < 256; ++i)
		itable[table[i]] = i;
}

void print_table() {
	for (unsigned char *p = table; *p; ++p)
		if (*p == '\n')
			printf(" '\\n',");
		else if (*p == '\t')
			printf(" '\\t',");
		else if (*p == '\'')
			printf(" '\\\'',");
		else if (*p == '\\')
			printf(" '\\\\',");
		else if (*p >= 32 && *p < 127)
			printf(" '%c',", *p);
		else
			printf(" %d,", *p);
	printf(" 0\n");
}

void move_to_front(int symbol) {
	for (int i = itable[symbol]; i; --i)
		itable[table[i] = table[i - 1]] = i;
	itable[table[0] = symbol] = 0;
}

int get_symbol(int value) {
	int symbol = table[value];
	move_to_front(symbol);
	return symbol;
}

int get_value(int symbol) {
	int value = itable[symbol];
	move_to_front(symbol);
	return value;
}

