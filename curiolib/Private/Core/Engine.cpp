#include "Core/Engine.h"
#include "Log/Log.h"
#include "Memory/Memory.h"
#include "Core/Application.h"

#ifdef CU_PLATFORM_SDL
#include "SDL3/SDL.h"
#include "Platform/WindowSDL.h"
#endif

using namespace Core;

void Engine::Initialize(int argc, char** argv)
{
	// Initialize engine core globals before anything else
	CuLog::GLog::Get().Initialize();
	GMemory::Get().Initialize();

	// Platform initialization
#ifdef CU_PLATFORM_SDL
	SDL_Init(SDL_INIT_VIDEO);
	CU_LOG_ENGINE(Info, "SDL Initialized");
	WindowHandle = NewObject<Platform::WindowSDL>();
#endif

	// App creation - should give us access to app spec
	AppHandle = CreateApplication(argc, argv);
	
	// Window creation
	CU_LOG_ENGINE(Debug, "Application spec %s found", AppHandle->GetAppSpec().Name);
	WindowHandle->Create({AppHandle->GetAppSpec().Name, AppHandle->GetAppSpec().Width, AppHandle->GetAppSpec().Height});

	// App initialization
	AppHandle->Initialize();

	// Last, set the initialized flag to true
	Initialized = true;
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

	// Cleanup
	DeleteObject<Application>(AppHandle);	
	DeleteObject<Platform::IWindow>(WindowHandle);
	
	// Engine core globals
	Core::GMemory::Get().Shutdown();
	CuLog::GLog::Get().Shutdown();

	Initialized = false;
}

bool Engine::ShouldQuit() const
{
	return CanQuit || WindowHandle->ShouldClose();
}

Engine::~Engine()
{
	if (Initialized)
		Shutdown();
}
