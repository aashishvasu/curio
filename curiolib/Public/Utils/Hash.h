#pragma once

namespace Utils
{
	// Integer hashing (SplitMix64 - excellent distribution)
	inline uint64 Hash64(uint64 Value)
	{
		Value = (Value ^ (Value >> 30)) * 0xbf58476d1ce4e5b9ULL;
		Value = (Value ^ (Value >> 27)) * 0x94d049bb133111ebULL;
		Value = Value ^ (Value >> 31);
		return Value;
	}

	// Pointer hashing
	template <typename T>
	inline uint64 Hash64(T* Pointer)
	{
		return Hash64(reinterpret_cast<uintptr>(Pointer));
	}

	// String hashing (FNV-1a)
	inline uint64 Hash64(const char* String)
	{
		if (!String) return 0;

		uint64 hash = 0xcbf29ce484222325ULL;
		for (const char* ptr = String; *ptr; ++ptr) {
			hash ^= static_cast<uint8>(*ptr);
			hash *= 0x100000001b3ULL;
		}
		return hash;
	}

	// Hash combiner
	inline uint64 Combine(uint64 Hash1, uint64 Hash2)
	{
		Hash1 ^= Hash2 + 0x9e3779b97f4a7c15ULL + (Hash1 << 6) + (Hash1 >> 2);
		return Hash1;
	}
}
