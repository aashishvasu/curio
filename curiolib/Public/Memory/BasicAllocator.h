#pragma once

#include "Allocator.h"
#include <atomic>

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
		size_t GetTotalAllocated() const override { return TotalAllocated.load(std::memory_order_relaxed); }
		// Allocator interface
		size_t GetNumAllocations() const { return NumAllocations.load(std::memory_order_relaxed); }

	private:
		struct AllocationHeader
		{
			size_t Size = 0;
			uint16 Offset = 0;
#if defined(CURIO_DEBUG) || defined(CURIO_RELDBG)
			uint16 Magic = 0xBEEF;
#endif
		};

		std::atomic<size_t> TotalAllocated{0};
		std::atomic<size_t> NumAllocations{0};
	};
}
