#include "Core/Log.h"

#include <mutex>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

using namespace CuLog;

// spdlog impl
namespace
{
	// Struct for static get
	struct LoggerRegistry
	{
		std::mutex mutex;
		std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> loggers;

		static LoggerRegistry& Get()
		{
			static LoggerRegistry instance;
			return instance;
		}
	};
	
    
	spdlog::level::level_enum ToSpdlogLevel(CuLog::ELogLevel level) {
		switch (level) {
		case ELogLevel::Trace:		return spdlog::level::trace;
		case ELogLevel::Debug:		return spdlog::level::debug;
		case ELogLevel::Info:		return spdlog::level::info;
		case ELogLevel::Warning:	return spdlog::level::warn;
		case ELogLevel::Error:		return spdlog::level::err;
		case ELogLevel::Critical:	return spdlog::level::critical;
		}
		return spdlog::level::info;
	}
}

GLog& GLog::Get()
{
	static GLog instance;
	return instance;
}

void GLog::Initialize()
{
	if (Initialized)
		return;
	
	// Create engine SLogger
	EngineLogger = RegisterLogger("Curio", ELogLevel::Trace);

	// Set verbosity
	SetLogVerbosity(EVerbosity::Verbose);

	Initialized = true;
}

void GLog::Shutdown()
{
	if (!Initialized)
		return;

	{
		std::lock_guard<std::mutex> lock(LoggerRegistry::Get().mutex);
		LoggerRegistry::Get().loggers.clear();
	}
	spdlog::shutdown();
	Initialized = false;
}

void GLog::SetLogVerbosity(const EVerbosity& inVerbosity)
{
	LoggerVerbosity = inVerbosity;
	
	switch (LoggerVerbosity)
	{
	case EVerbosity::Verbose:
		spdlog::set_pattern("%^[%8l][%Y-%m-%d %T][%n][%s:%#]: %v%$");
		break;
	case EVerbosity::Normal:
		spdlog::set_pattern("%^[%8l][%T][%n]: %v%$");
		break;
	case EVerbosity::Terse:
		spdlog::set_pattern("%^[%L]: %v%$");
		break;
	}
}

SLogger GLog::RegisterLogger(const char* inName, const ELogLevel& inLevel)
{
	SLogger newLogger = {inName, inLevel};
	// TODO: Add to CuArray when that is ready
	
	// Create spdlog logger
	// TODO: Use CuString when it's ready
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(std::string(newLogger.Name)+".log", true);
    
	// Create logger
	auto spd_logger = std::make_shared<spdlog::logger>(newLogger.Name, spdlog::sinks_init_list{console_sink, file_sink});
	spd_logger->set_level(ToSpdlogLevel(inLevel));
	spd_logger->flush_on(spdlog::level::err);
    
	spdlog::register_logger(spd_logger);
	LoggerRegistry::Get().loggers[newLogger.Name] = spd_logger;

	return newLogger;
}

const char* GLog::GetBaseFilename(const char* FilePath)
{
	if (!FilePath)
		return "";
		
	// Find the last slash or backslash
	const char* LastSlash = strrchr(FilePath, '/');
	if (!LastSlash)
	{
		LastSlash = strrchr(FilePath, '\\');
	}
		
	// If we found a slash, return the character after it
	// Otherwise return the original string
	return LastSlash ? LastSlash + 1 : FilePath;
}

void GLog::LogEngine(ELogLevel inLevel, const char* inFile, int inLine, const char* Format, ...)
{
	va_list args;
	va_start(args, Format);
	LogRaw(EngineLogger, LoggerVerbosity, inLevel, inFile, inLine, Format, args);
	va_end(args);
}

void GLog::Log(const SLogger& inLogger, ELogLevel inLevel, const char* inFile, int inLine, const char* Format, ...)
{
	va_list args;
	va_start(args, Format);
	LogRaw(inLogger, LoggerVerbosity, inLevel, inFile, inLine, Format, args);
	va_end(args);
}

void GLog::LogRaw(const SLogger& inLogger, const EVerbosity& inVerbosity, const ELogLevel& inLevel, const char* inFile, int inLine, const char* Format, va_list args)
{
	if (!Initialized)
		Initialize();

	if (inLevel < inLogger.Level)
		return;

	// Get or create logger
	auto it = LoggerRegistry::Get().loggers.find(inLogger.Name);
	if (it == LoggerRegistry::Get().loggers.end())
	{
		// Create new logger with same sinks as engine logger
		auto engine_it = LoggerRegistry::Get().loggers.find(EngineLogger.Name);
		if (engine_it != LoggerRegistry::Get().loggers.end())
		{
			auto new_logger = std::make_shared<spdlog::logger>(inLogger.Name, engine_it->second->sinks().begin(), engine_it->second->sinks().end());
			spdlog::register_logger(new_logger);
			LoggerRegistry::Get().loggers[inLogger.Name] = new_logger;
			it = LoggerRegistry::Get().loggers.find(inLogger.Name);
		}
		else
		{
			return;
		}
	}

	// Format the message
	char buffer[4096];
	(void)vsnprintf(buffer, sizeof(buffer), Format, args);
    
	// Log with source location
	const char* filename = GetBaseFilename(inFile);
	it->second->log(spdlog::source_loc{filename, inLine, ""}, ToSpdlogLevel(inLevel), buffer);
}
