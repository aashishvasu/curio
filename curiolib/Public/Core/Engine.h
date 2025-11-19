#pragma once

namespace CuCore
{
	class CURIO_API Engine
	{
	public:
		void Initialize(int argc, char** argv);
		void Update();
		void Shutdown();
	};
}

