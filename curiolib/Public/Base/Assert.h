#pragma once

#include "Core/Log.h"

// Enable asserts in both Debug and RelWithDebInfo builds
#if defined(CURIO_DEBUG) || defined(CURIO_RELDBG)
	#define CURIO_ENABLE_ASSERTS 1
#else
	#define CURIO_ENABLE_ASSERTS 0
#endif

#if CURIO_ENABLE_ASSERTS
	#if defined(_MSC_VER)
		#define CU_DEBUG_BREAK() __debugbreak()
	#elif defined(__clang__) || defined(__GNUC__)
		#define CU_DEBUG_BREAK() __builtin_trap()
	#else
		#define CU_DEBUG_BREAK() abort()
	#endif

	#define CU_ASSERT(condition, ...) \
	do \
	{ \
		if (!(condition)) \
		{ \
			CU_LOG_ENGINE(Critical, "Assertion failed: %s", #condition); \
			if (""#__VA_ARGS__[0]) \
				CU_LOG_ENGINE(Critical, __VA_ARGS__); \
			CU_DEBUG_BREAK(); \
		} \
	} while(0)

	// Useful for marking code that should never be reached
	#define CU_ASSERT_NO_ENTRY() CU_ASSERT(false, "This code path should never be reached!")

	// Useful for marking unimplemented code
	#define CU_ASSERT_NOT_IMPLEMENTED() CU_ASSERT(false, "Function not yet implemented!")

#else
	#define CU_ASSERT(condition, ...)
	#define CU_ASSERT_NO_ENTRY()
	#define CU_ASSERT_NOT_IMPLEMENTED()
#endif
