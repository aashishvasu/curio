#pragma once

#include "Allocator.h"

namespace Core
{
	class CURIO_API BasicAllocator final : public IAllocator
	{
	public:
		BasicAllocator() = default;
		~BasicAllocator() override;

		// Allocator interface
		void* Allocate(size_t Size, size_t Alignment) override;
		void Free(void* Ptr) override;
		size_t GetTotalAllocated() const override { return TotalAllocated; }
		// Allocator interface

	private:
		struct AllocationHeader
		{
			size_t Size;
			size_t Alignment;
		};

		size_t TotalAllocated = 0;
		size_t NumAllocations = 0;
	};
}
