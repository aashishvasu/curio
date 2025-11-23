#pragma once

#include <new>
#include "BasicAllocator.h"

namespace CuCore
{
	class CURIO_API GMemory
	{
	public:
		GMemory() = default;
		GMemory(const GMemory&) = delete;
		GMemory& operator=(const GMemory&) = delete;

		static GMemory& Get();
		
		void Initialize();
		void Shutdown();

		IAllocator& GetDefaultAllocator() const;
		void SetDefaultAllocator(IAllocator& Allocator);

	private:
		bool Initialized = false;
		BasicAllocator DefaultAllocator;
		IAllocator* CurrentAllocator = nullptr;
	};

	template <typename T, typename... Args>
	T* NewObject(Args... args)
	{
		void* memory = GMemory::Get().GetDefaultAllocator().Allocate(sizeof(T));
		return ::new(memory) T(args...);
	}

	template <typename T>
	void DeleteObject(T* Object)
	{
		if (Object)
		{
			Object->~T();
			GMemory::Get().GetDefaultAllocator().Free(Object);
		}
	}
}

// Basic allocation macros
#define CU_MALLOC(size) CuCore::GMemory::Get().GetDefaultAllocator().Allocate(size)
#define CU_MALLOC_ALIGNED(size, alignment) CuCore::GMemory::Get().GetDefaultAllocator().Allocate(size, alignment)
#define CU_FREE(ptr) CuCore::GMemory::Get().GetDefaultAllocator().Free(ptr)
