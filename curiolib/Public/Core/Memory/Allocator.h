#pragma once

#include "MemoryCommon.h"

namespace CuCore
{
	class CURIO_API IAllocator
	{
	public:
		virtual ~IAllocator() = default;
		virtual void* Allocate(size_t Size, size_t Alignment = DEFAULT_ALIGNMENT) = 0;
		virtual void Free(void* Ptr) = 0;
		virtual size_t GetTotalAllocated() const = 0;
	};
}
