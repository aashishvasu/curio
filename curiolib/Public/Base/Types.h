#pragma once

#include <cstdint>

// Integer types
typedef int8_t          int8;
typedef int16_t         int16;
typedef int32_t         int32;
typedef int64_t         int64;
typedef uint8_t         uint8;
typedef uint16_t        uint16;
typedef uint32_t        uint32;
typedef uint64_t        uint64;

typedef unsigned int	uint;

// Pointer types
typedef uintptr_t       uintptr;

// Floating point types
typedef float           float32;
typedef double          float64;

// Compile-time assertions to verify assumptions
static_assert(sizeof(int8)	== 1, "int8 must be 1 byte");
static_assert(sizeof(int16)	== 2, "int16 must be 2 bytes");
static_assert(sizeof(int32)	== 4, "int32 must be 4 bytes");
static_assert(sizeof(int64)	== 8, "int64 must be 8 bytes");

namespace CuCore
{
	constexpr int32 INDEX_NONE = -1;
}
