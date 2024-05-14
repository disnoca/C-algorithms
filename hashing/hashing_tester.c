#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "hashing.h"

static void test_CRC32(void) {
	assert(CRC32("Hello World", 11) == 0x4A17B156);
    assert(CRC32("Hello World!", 12) == 0x1C291CA3);
    assert(CRC32("Hello world", 11) == 0x8BD69E52);
    assert(CRC32("Hello world!", 12) == 0x1B851995);
}

int main(void) {
	test_CRC32();

	printf("All tests passed.\n");
	return 0;
}
