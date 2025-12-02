#pragma once

#include "Core/Engine.h"

#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)
	inline int main(int argc, char** argv)
	{
		// Stack allocate for now
		Core::Engine engine = Core::Engine();

		// Initialization
		engine.Initialize(argc, argv);

		// Update
		while (engine.ShouldQuit() == false)
		{
			engine.Update();
		}

		// Shutdown
		engine.Shutdown();
		
		return 0;
	}

#else
	#error Curio only supports Windows, Linux, and macOS!
#endif
