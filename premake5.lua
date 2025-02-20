workspace "CurioEngine"
	configurations { "Debug", "Release", "Dist" }
	architecture "x64"
	warnings "Default"

	filter "configurations:Debug"
		optimize "Off"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "On"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"
		linktimeoptimization "On"

project "Curio"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	location "%{wks.location}/Intermediate/ProjectFiles"
	targetdir "%{wks.location}/Binaries/%{cfg.system}%{cfg.architecture}/%{cfg.buildcfg}"
	objdir "%{wks.location}/Intermediate"

	pchheader "CuPCH.h"
	pchsource "Curio/src/CuPCH.cpp"

	files {
		"Curio/src/**.cpp",
		"Curio/include/**.h",
		"Curio/include/**.hpp"
	}

	includedirs {
		"Curio/include",
		"Curio/include/Core",
		"Curio/include/Utils",

		"Extern/spdlog/include"
	}

	defines { "CURIO_BUILD_DLL" }

	-- Flags
	flags { "MultiProcessorCompile" }

	-- Configs
	filter "configurations:Debug"
		defines { "CURIO_DEBUG", "DEBUG" }

	filter "configurations:Release"
		defines { "CURIO_RELEASE", "NDEBUG" }

	filter "configurations:Dist"
		defines { "CURIO_DIST", "NDEBUG" }

	-- Systems
	filter "system:windows"
		defines {
			"CURIO_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS"
		}
		buildoptions { "/utf-8" }

	filter "system:linux"
		defines { "CURIO_LINUX" }

	filter "system:macosx"
		defines { "CURIO_MACOS" }
