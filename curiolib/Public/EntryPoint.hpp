#pragma once

#include "Core/Engine.h"
#include "Core/Application.h"

extern CuCore::Application* CuCore::CreateApplication();

#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)
	inline int main(int argc, char** argv)
	{
		// TODO: Use custom allocator when ready
		CuCore::Engine* engine = new CuCore::Engine();
		CuCore::Application* app = CuCore::CreateApplication();

		// Initialization
		engine->Initialize(argc, argv);
		app->Initialize();

		// Update
		// while (true)
		// {
		// 	engine->Update();
		// 	app->Update();
		// }

		// Shutdown
		app->Shutdown();
		engine->Shutdown();
		
		delete app;
		delete engine;
		return 0;
	}

#else
	#error Curio only supports Windows, Linux, and macOS!
#endif
