// Curio Precompiled Header

#pragma once

// Standard library goes here
#include <chrono>
#include <memory>

// Common includes
#include "CuAPI.h"

// Platform-specific headers
#ifdef CURIO_WINDOWS
	#include <Windows.h>
#elif defined(CURIO_LINUX)
	#include <unistd.h>
#elif defined(CURIO_MACOS)
	#include <mach-o/dyld.h>
#endif
