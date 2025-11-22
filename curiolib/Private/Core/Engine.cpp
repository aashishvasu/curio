#include "Core/Engine.h"

#include "Core/Log.h"

#ifdef CU_PLATFORM_SDL
#include "SDL3/SDL.h"
#include "Core/Platform/CuWindowSDL.h"
#endif

using namespace CuCore;

void Engine::Initialize(int argc, char** argv)
{
	// Initialize logger
	CuLog::GLog::Get().Initialize();

	// Platform initialization
#ifdef CU_PLATFORM_SDL
	SDL_Init(SDL_INIT_VIDEO);
	CU_LOG_ENGINE(Info, "SDL Initialized");
	WindowHandle = new CuWindowSDL();
#endif
	// Window creation
	WindowHandle->Create("Curio", 640, 480);
}

void Engine::Update()
{
	// Poll window events
	// TODO: Temp implementation in lieu of event system
	WindowHandle->PollEvents();
}

void Engine::Shutdown()
{
	CU_LOG_ENGINE(Info, "CuCore::Engine::Shutdown called");
	
	// Shutdown in reverse order
	WindowHandle->Close();
	delete WindowHandle;
#ifdef CU_PLATFORM_SDL
	SDL_Quit();
#endif
	CuLog::GLog::Get().Shutdown();
}

bool Engine::ShouldQuit() const
{
	return CanQuit || WindowHandle->ShouldClose();
}
