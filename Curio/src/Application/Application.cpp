#include "CuPCH.h"

#include "Application/Application.h"
#include "Log/Log.h"

namespace Curio
{
	void Application::Initialize()
	{
		// Initialize logger
		Log::Init();
		CU_LOG(info, "Logger Initialized");
	}

	int Application::Run(int argc, char* argv[])
	{
		//Init
		Initialize();
	
		auto lastTime = std::chrono::high_resolution_clock::now();

		// Loop
		while (true)
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;

			Update(deltaTime);
		}

		Shutdown();
		return 0;
	}
}
