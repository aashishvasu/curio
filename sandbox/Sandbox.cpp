#include "Sandbox.h"

Sandbox::Sandbox(const Core::ApplicationSpec& Spec) : Application(Spec) {}

void Sandbox::Initialize()
{
	CU_LOG(LogSandbox, Trace, "App initialized");

	CU_LOG(LogSandbox, Trace, "Total memory allocated: %d", Core::GMemory::Get().GetDefaultAllocator().GetTotalAllocated());
}

void Sandbox::Update()
{
	
}

void Sandbox::Shutdown()
{
	CU_LOG(LogSandbox, Trace, "App shutdown");
}
