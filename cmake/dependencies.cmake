include(FetchContent)
# We set custom extern directory
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/extern)

# SDL3
FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG release-3.2.26
	FIND_PACKAGE_ARGS GLOBAL
)

# spdlog
FetchContent_Declare(
	spdlog
	GIT_REPOSITORY https://github.com/gabime/spdlog.git
	GIT_TAG v1.16.0
	FIND_PACKAGE_ARGS GLOBAL
)

# Fetch content
FetchContent_MakeAvailable(SDL3 spdlog)