#include "Core/Engine.h"

#include "Core/Log.h"

using namespace CuCore;

void Engine::Initialize(int argc, char** argv)
{
	// Initialize logger
	CuLog::GLog::Get().Initialize();
	
	CU_LOG_ENGINE(Info, "CuCore::Engine::Initialize called");
}

void Engine::Update()
{
	
}

void Engine::Shutdown()
{
	CU_LOG_ENGINE(Info, "CuCore::Engine::Shutdown called");
	
	// Shutdown in reverse order
	CuLog::GLog::Get().Shutdown();
}
