#pragma once

#include "spdlog/spdlog.h"

namespace Curio
{
	class CURIO_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return EngineLogger; }

	private:
		static std::shared_ptr<spdlog::logger> EngineLogger;
	};
}

#ifndef CURIO_DIST
	#define CU_LOG(verbosity, ...) ::Curio::Log::GetLogger()->verbosity(__VA_ARGS__)
#else
	#define CU_LOG(verbosity, ...)
#endif