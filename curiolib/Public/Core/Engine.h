#pragma once

namespace Platform
{
	class IWindow;
}

namespace Core
{
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
		Platform::IWindow* WindowHandle = nullptr;

		bool CanQuit = false;
		bool Initialized = false;
	};
}
