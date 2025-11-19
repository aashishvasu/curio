#pragma once

namespace CuCore
{
	class CURIO_API Application
	{
	public:
		virtual ~Application() = default;
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;
	};

	// To be defined in project
	Application* CreateApplication();
}