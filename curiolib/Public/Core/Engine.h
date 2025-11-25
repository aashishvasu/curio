#pragma once

namespace CuCore
{
	class IWindow;
	class Application;

	extern Application* CreateApplication(int argc, char** argv);
	
	class CURIO_API Engine
	{
	public:
		Engine() = default;
		~Engine();
		void Initialize(int argc, char** argv);
		void Update();
		void Shutdown();

		bool ShouldQuit() const;

	private:

	private:
		// Application handle
		Application* AppHandle = nullptr;
		
		// The main window
		IWindow* WindowHandle = nullptr;

		bool CanQuit = false;
		bool Initialized = false;
	};
}
