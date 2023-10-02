#pragma once
#include <cstdint>

static uint64_t CantorFunction(uint64_t a, uint64_t b) {
	return (((a + b) * (a + b + 1)) + b) >> 1;
}