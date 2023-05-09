#include "hashing.h"

#define REVERSE_POLYNOMIAL 	0xEDB88320
#define START_VALUE 		0xFFFFFFFF
#define FINAL_XOR_VALUE 	0xFFFFFFFF


uint32_t CRC32(const char* data, unsigned length) {
	uint32_t crc = START_VALUE;

	for (unsigned i = 0; i < length; i++) {
		crc ^= data[i];
		for (unsigned j = 0; j < 8; j++)
			crc = (crc >> 1) ^ (REVERSE_POLYNOMIAL & -(crc & 1));
	}

	return crc ^ FINAL_XOR_VALUE;
}