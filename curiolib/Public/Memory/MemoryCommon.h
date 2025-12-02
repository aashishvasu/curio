#pragma once

namespace Core
{
	// Memory constants
	constexpr size_t DEFAULT_ALIGNMENT = 8;
	constexpr size_t KB = 1024;
	constexpr size_t MB = 1024 * KB;
	constexpr size_t GB = 1024 * MB;

	// Utility functions
	constexpr bool IsPowerOfTwo(const size_t X) { return X && !(X & (X - 1)); }

	inline size_t AlignForwardAdjustment(const void* Address, const size_t Alignment)
	{
		CU_ASSERT(IsPowerOfTwo(Alignment), "Alignment must be power of 2");
		const uintptr_t addressValue = reinterpret_cast<uintptr_t>(Address);
		const uintptr_t mask = Alignment - 1;
		return (Alignment - (addressValue & mask)) & mask;
	}
}
