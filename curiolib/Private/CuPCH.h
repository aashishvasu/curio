#pragma once

// System includes

// Curio includes
#include "Base/CuAPI.h"
#include "Base/Types.h"
#include "Base/Assert.h"

// Platform includes
#ifdef _WIN32
	#include <Windows.h>
#elif defined(__linux__)
	#ifdef USE_WAYLAND
		#include <wayland-client.h>
	#else
		#include <X11/Xlib.h>
	#endif
#elif defined(__APPLE__)
	#include <CoreFoundation/CoreFoundation.h>
#endif