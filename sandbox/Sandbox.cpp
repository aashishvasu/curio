#include "Sandbox.h"

Sandbox::Sandbox(const CuCore::ApplicationSpec& Spec) : Application(Spec) {}

void Sandbox::Initialize()
{
	CU_LOG(LogSandbox, Trace, "App initialized");
}

void Sandbox::Update()
{
	
}

void Sandbox::Shutdown()
{
	CU_LOG(LogSandbox, Trace, "App shutdown");
}
