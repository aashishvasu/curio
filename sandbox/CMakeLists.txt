# Collect headers and source
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}*.hpp")

file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}*.cpp")

add_executable(sandbox ${SOURCE_LIST} ${HEADER_LIST})
target_link_libraries(sandbox PUBLIC curiolib)

# Copy sdl shared lib
add_custom_command(TARGET sandbox POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:SDL3::SDL3>
		$<TARGET_FILE_DIR:sandbox>)