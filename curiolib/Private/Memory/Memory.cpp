#include "CuPCH.h"

#include "Memory/Memory.h"

using namespace Core;

GMemory& GMemory::Get()
{
	static GMemory instance;
	return instance;
}

void GMemory::Initialize()
{
	if (Initialized)
		return;
	
	CurrentAllocator = &DefaultAllocator;
	
	Initialized = true;
	
	CU_LOG_ENGINE(Trace, "Global memory module initialized");
}

void GMemory::Shutdown()
{
	if (!Initialized)
		return;

	CurrentAllocator = nullptr;
	Initialized = false;
	CU_LOG_ENGINE(Trace, "Global memory module shut down");
}

IAllocator& GMemory::GetDefaultAllocator() const
{
	CU_ASSERT(Initialized, "Memory module not initialized!");
	return *CurrentAllocator;
}

void GMemory::SetDefaultAllocator(IAllocator& Allocator)
{
	CU_ASSERT(Initialized, "Memory module not initialized!");
	CurrentAllocator = &Allocator;
}