#include "CuPCH.h"

#include "Log/Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Curio
{
	std::shared_ptr<spdlog::logger> Log::EngineLogger;
	
	void Log::Init()
	{
		spdlog::set_pattern("%^[%L][%D %T] [%n]: %v%$");
		EngineLogger = spdlog::stderr_color_mt("Curio");
		EngineLogger->set_level(spdlog::level::trace);
	}
}

