#include <stdio.h>
#include <stdbool.h>
#include "hashing.h"

bool test_CRC32(void) {
	bool result = true;

	result = result && CRC32("Hello World", 11) == 0x4A17B156;
    result = result && CRC32("Hello World!", 12) == 0x1C291CA3;
    result = result && CRC32("Hello world", 11) == 0x8BD69E52;
    result = result && CRC32("Hello world!", 12) == 0x1B851995;

	return result;
}

int main(void) {

	printf("CRC32: %d\n", test_CRC32());

	return 0;
}