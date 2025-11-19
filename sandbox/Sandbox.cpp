#include "Sandbox.h"

void Sandbox::Initialize()
{
	CU_LOG(LogSandbox, Trace, "Trace");
	CU_LOG(LogSandbox, Debug, "Debug");
	CU_LOG(LogSandbox, Info, "Info");
	CU_LOG(LogSandbox, Warning, "Warning");
	CU_LOG(LogSandbox, Error, "Error");
	CU_LOG(LogSandbox, Critical, "Critical");
}

void Sandbox::Update()
{
	
}

void Sandbox::Shutdown()
{
	
}
