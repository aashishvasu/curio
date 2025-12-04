#include "Memory/BasicAllocator.h"
#include <cstdlib>

using namespace Core;

BasicAllocator::~BasicAllocator()
{
	// Log the memory leak
	CU_ASSERT(NumAllocations == 0, "Memory leak detected! %zu allocations totaling %zu bytes not freed.", NumAllocations.load(std::memory_order_relaxed), TotalAllocated.load(std::memory_order_relaxed));
}

void* BasicAllocator::Allocate(const size_t Size, const size_t Alignment)
{
	CU_ASSERT(IsPowerOfTwo(Alignment), "Alignment must be power of 2");
	CU_ASSERT(Alignment <= 65536, "Alignment exceeds maximum supported (64KB)");

	// Zero size allocation should return nullptr
	if (Size == 0)
		return nullptr;

	// Calculate total allocation size
	const size_t totalSize = sizeof(AllocationHeader) + Size + (Alignment - 1);

	// Wrap basic malloc. Around malloc is size and reference counting
	void* rawMemory = malloc(totalSize);
	CU_ASSERT(rawMemory != nullptr, "Out of memory, failed to allocate %d bytes", totalSize);
	if (!rawMemory)
		return nullptr;

	// Calculate where the user pointer should be (after header + alignment)
	const uintptr rawAddr = reinterpret_cast<uintptr>(rawMemory);
	uintptr userAddr = rawAddr + sizeof(AllocationHeader);
	const size_t alignmentOffset = AlignForwardAdjustment(reinterpret_cast<void*>(userAddr), Alignment);
	userAddr += alignmentOffset;

	// Place header immediately before user pointer
	AllocationHeader* header = reinterpret_cast<AllocationHeader*>(userAddr - sizeof(AllocationHeader));
	header->Size = Size;
	header->Offset = static_cast<uint16>(userAddr - rawAddr);
#if defined(CURIO_DEBUG) || defined(CURIO_RELDBG)
	header->Magic = 0xBEEF;
#endif

	// Update statistics
	NumAllocations.fetch_add(1, std::memory_order_relaxed);
	TotalAllocated.fetch_add(Size, std::memory_order_relaxed);

	return reinterpret_cast<void*>(userAddr);
}


void BasicAllocator::Free(void* Ptr)
{
	if (!Ptr)
		return;

	// Get header from before the user pointer
	AllocationHeader* header = reinterpret_cast<AllocationHeader*>(reinterpret_cast<uintptr>(Ptr) - sizeof(AllocationHeader));

#if defined(CURIO_DEBUG) || defined(CURIO_RELDBG)
	CU_ASSERT(header->Magic == 0xBEEF, "Memory corruption detected: Invalid magic number");
	// Stomp the magic to catch double free
	header->Magic = 0xDEAD;
#endif

	// Calculate original malloc pointer
	void* rawPtr = reinterpret_cast<void*>(reinterpret_cast<uintptr>(Ptr) - header->Offset);

	// Wrap basic free. Around free is reducing size and reference count
	NumAllocations.fetch_sub(1, std::memory_order_relaxed);
	TotalAllocated.fetch_sub(header->Size, std::memory_order_relaxed);

	// Free the original allocation
	free(rawPtr);
}