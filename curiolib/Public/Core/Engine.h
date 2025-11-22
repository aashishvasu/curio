#pragma once

namespace CuCore
{
	class CuWindow;	
	
	class CURIO_API Engine
	{
	public:
		void Initialize(int argc, char** argv);
		void Update();
		void Shutdown();

		bool ShouldQuit() const;

	private:
		// The main window
		// TODO: Replace with custom allocator
		CuWindow* WindowHandle = nullptr;

		bool CanQuit = false;
	};
}
