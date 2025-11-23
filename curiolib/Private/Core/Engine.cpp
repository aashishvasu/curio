#include "Core/Engine.h"
#include "Core/Log.h"
#include "Core/Memory/Memory.h"
#include "Core/Application.h"

#ifdef CU_PLATFORM_SDL
#include "SDL3/SDL.h"
#include "Core/Platform/WindowSDL.h"
#endif

using namespace CuCore;

void Engine::Initialize(int argc, char** argv)
{
	// Initialize global engine modules
	CuLog::GLog::Get().Initialize();
	CuCore::GMemory::Get().Initialize();

	// Platform initialization
#ifdef CU_PLATFORM_SDL
	SDL_Init(SDL_INIT_VIDEO);
	CU_LOG_ENGINE(Info, "SDL Initialized");
	WindowHandle = new WindowSDL();
#endif

	// App creation - should give us access to app spec
	AppHandle = CreateApplication(argc, argv);
	
	// Window creation
	CU_LOG_ENGINE(Debug, "Application spec %s found", AppHandle->GetAppSpec().Name);
	WindowHandle->Create({AppHandle->GetAppSpec().Name, AppHandle->GetAppSpec().Width, AppHandle->GetAppSpec().Height});

	// App initialization
	AppHandle->Initialize();
}

void Engine::Update()
{
	AppHandle->Update();
	
	// Poll window events
	// TODO: Temp implementation in lieu of event system
	WindowHandle->PollEvents();
}

void Engine::Shutdown()
{
	CU_LOG_ENGINE(Trace, "Engine shutdown begin");
	
	// Shutdown in reverse order
	// App
	AppHandle->Shutdown();
	
	// Window
	WindowHandle->Close();

	// Platform
#ifdef CU_PLATFORM_SDL
	SDL_Quit();
#endif
	
	// Engine globals
	CuCore::GMemory::Get().Shutdown();
	CuLog::GLog::Get().Shutdown();

	// Cleanup
	delete AppHandle;
	delete WindowHandle;
}

bool Engine::ShouldQuit() const
{
	return CanQuit || WindowHandle->ShouldClose();
}
