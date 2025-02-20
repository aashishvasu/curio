#pragma once

namespace Curio
{
	class CURIO_API Application
	{
	public:
		Application() = default;
		virtual void Initialize();
		virtual void Update(float DeltaTime) = 0;
		virtual void Shutdown() = 0;
		
		virtual int Run(int argc, char* argv[]);
	};
}
