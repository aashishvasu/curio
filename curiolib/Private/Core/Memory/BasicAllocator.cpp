#include "CuPCH.h"

#include "Core/Memory/BasicAllocator.h"

using namespace CuCore;

BasicAllocator::~BasicAllocator()
{
	// Log the memory leak
	CU_ASSERT(NumAllocations == 0, "Memory leak detected! %d allocations totaling %d bytes not freed.", NumAllocations, TotalAllocated);
}

void* BasicAllocator::Allocate(const size_t Size, const size_t Alignment)
{
	CU_ASSERT(Size > 0, "Attempting to allocate 0 bytes");
	CU_ASSERT(Alignment > 0 && IsPowerOfTwo(Alignment), "Alignment must be non-zero power of 2");
	
	size_t headerSize = sizeof(AllocationHeader);
	size_t totalSize = Size + headerSize;

	// Wrap basic malloc. Around malloc is size and reference counting
	void* rawMemory = malloc(totalSize);
	CU_ASSERT(rawMemory != nullptr, "Out of memory, failed to allocate %d bytes", totalSize);
	if (!rawMemory)
		return nullptr;

	AllocationHeader* header = static_cast<AllocationHeader*>(rawMemory);
	header->Size = Size;
	header->Alignment = Alignment;

	TotalAllocated += Size;
	NumAllocations++;

	return static_cast<char*>(rawMemory) + headerSize;
}

void BasicAllocator::Free(void* Ptr)
{
	if (!Ptr)
		return;

	void* headerPtr = static_cast<char*>(Ptr) - sizeof(AllocationHeader);
	AllocationHeader* header = static_cast<AllocationHeader*>(headerPtr);

	CU_ASSERT(header->Size > 0, "Corrupted allocation header detected");
	CU_ASSERT(header->Size <= TotalAllocated, "Memory corruption: freeing more than allocated");

	// Wrap basic free. Around free is reducing size and reference count
	TotalAllocated -= header->Size;
	NumAllocations--;

	free(headerPtr);
}
