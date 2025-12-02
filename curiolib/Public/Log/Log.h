#pragma once

namespace CuLog
{
	enum class CURIO_API ELogLevel : uint8
	{
		Trace = 0,
		Debug,
		Info,
		Warning,
		Error,
		Critical
	};

	enum class CURIO_API EVerbosity : uint8
	{
		Verbose = 0,
		Normal,
		Terse
	};

	struct CURIO_API SLogger
	{
		const char* Name;
		ELogLevel Level = ELogLevel::Info;
	};

	class CURIO_API GLog final
	{
	public:
		GLog() = default;
		~GLog() = default;
		GLog(const GLog&) = delete;
		GLog& operator=(const GLog&) = delete;
		
		static GLog& Get();
		
		void Initialize();
		void Shutdown();

		void SetLogVerbosity(const EVerbosity& inVerbosity);
		static SLogger RegisterLogger(const char* inName, const ELogLevel& inLevel);
    
		// Helper to extract the base filename from a path
		static const char* GetBaseFilename(const char* FilePath);

		void LogEngine(ELogLevel inLevel, const char* inFile, int inLine, const char* Format, ...);
		void Log(const SLogger& inLogger, ELogLevel inLevel, const char* inFile, int inLine,  const char* Format, ...);
		
	private:
		void LogRaw(const SLogger& inLogger, const EVerbosity& inVerbosity, const ELogLevel& inLevel, const char* inFile, int inLine, const char* Format, va_list args);
		
		bool Initialized = false;
		SLogger EngineLogger;
		EVerbosity LoggerVerbosity = EVerbosity::Normal;
	};
}

// Logging macros
#if defined(CURIO_DEBUG) || defined(CURIO_RELDBG)
    #define DECLARE_LOGGER(Name, LogLevel) \
	static CuLog::SLogger Name = CuLog::GLog::Get().RegisterLogger(#Name, CuLog::ELogLevel::LogLevel)
#else
    #define DECLARE_LOGGER(Name, DefaultLevel) \
	static CuLog::SLogger Name = {#Name, CuLog::ELogLevel::DefaultLevel}
#endif


// Logger usage
#if defined(CURIO_DEBUG) || defined(CURIO_RELDBG)
    #define CU_LOG_ENGINE(LogLevel, format, ...) \
	CuLog::GLog::Get().LogEngine(CuLog::ELogLevel::LogLevel, __FILE__, __LINE__, format, ##__VA_ARGS__)
    #define CU_LOG(Logger, LogLevel, format, ...) \
	CuLog::GLog::Get().Log(Logger, CuLog::ELogLevel::LogLevel, __FILE__, __LINE__, format, ##__VA_ARGS__)
#else
    #define CU_LOG_ENGINE(Level, format, ...) 
    #define CU_LOG(Logger, Level, format, ...) 
#endif
