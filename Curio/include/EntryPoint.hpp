#pragma once

#include "Application/Application.h"

#if defined(CURIO_WINDOWS)
	#define CURIO_MAIN(AppClass) \
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) { \
	AppClass* app = new AppClass(); \
	const int result = app->Run(__argc, __argv); \
	delete app; \
	return result; \
	}
#elif defined(CURIO_LINUX) || defined(CURIO_MACOS)
	#define CURIO_MAIN(AppClass) \
	int main(int argc, char* argv[]) { \
	AppClass* app = new AppClass(); \
	const int result = app->Run(argc, argv); \
	delete app; \
	return result; \
	}
#else
	#error "Unsupported platform"
#endif
