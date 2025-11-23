#pragma once

namespace CuCore
{
	class IWindow;
	class Application;

	extern Application* CreateApplication(int argc, char** argv);
	
	class CURIO_API Engine
	{
	public:
		void Initialize(int argc, char** argv);
		void Update();
		void Shutdown();

		bool ShouldQuit() const;

	private:
		// Application handle
		Application* AppHandle = nullptr;
		
		// The main window
		// TODO: Replace with custom allocator
		IWindow* WindowHandle = nullptr;

		bool CanQuit = false;
	};
}
