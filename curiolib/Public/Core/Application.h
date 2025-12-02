#pragma once

namespace Core
{
	struct CURIO_API AppCmdLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			CU_ASSERT(index < Count, "AppCmdLineArgs: Index out of range");
			return Args[index];
		}
	};
	
	struct CURIO_API ApplicationSpec
	{
		// TODO: Replace with custom string when ready
		const char* Name;
		int Width = -1;
		int Height = -1;
		AppCmdLineArgs Args;
	};
	
	class CURIO_API Application
	{
	public:
		Application(const ApplicationSpec& spec);
		virtual ~Application() = default;

		// Lifecycle
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;

		// Helpers
		ApplicationSpec GetAppSpec() const { return AppSpec; }
		
	private:
		ApplicationSpec AppSpec;
	};

	// To be defined in project
	Application* CreateApplication(int argc, char** argv);
}