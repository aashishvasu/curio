#pragma once

#ifdef CURIO_WINDOWS
	#ifdef CURIO_BUILD_DLL
		#define CURIO_API __declspec(dllexport)
	#else
		#define CURIO_API __declspec(dllimport)
	#endif
#elif defined(CURIO_LINUX) || defined(CURIO_MACOS)
	#ifdef CURIO_BUILD_DLL
		#define CURIO_API __attribute__((visibility("default")))
	#else
		#define CURIO_API
	#endif
#else
	#error Curio only supports Windows, Linux, and macOS!
#endif
