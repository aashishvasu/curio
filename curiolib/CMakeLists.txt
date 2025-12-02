include(GenerateExportHeader)

# curiolib
# Collect headers and source
file(GLOB_RECURSE PUBLIC_HEADER_LIST CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/*.hpp")

file(GLOB_RECURSE PUBLIC_SOURCE_LIST CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/*.cpp")

file(GLOB_RECURSE PRIVATE_HEADER_LIST CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/*.hpp")

file(GLOB_RECURSE PRIVATE_SOURCE_LIST CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/*.cpp")

# Add the library
add_library (curiolib
				${PUBLIC_HEADER_LIST}
				${PUBLIC_SOURCE_LIST}
				${PRIVATE_HEADER_LIST}
				${PRIVATE_SOURCE_LIST}
			)

# Generate export header
generate_export_header  (curiolib
							BASE_NAME CURIO
							EXPORT_MACRO_NAME CURIO_API
							EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/Public/Base/CuAPI.h
							STATIC_DEFINE CURIO_STATICLIB
						)

target_include_directories  (curiolib
								PUBLIC
									$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
									$<INSTALL_INTERFACE:include>
								PRIVATE
									${CMAKE_CURRENT_SOURCE_DIR}/Private
							)

target_compile_features(curiolib PUBLIC cxx_std_20)

target_link_libraries   (curiolib
							PRIVATE
								SDL3::SDL3
								spdlog::spdlog
						)

# Defines used by the library
target_compile_definitions  (curiolib
								PUBLIC
									$<$<CONFIG:Debug>:CURIO_DEBUG>
									$<$<CONFIG:RelWithDebInfo>:CURIO_RELDBG>
									$<$<CONFIG:Release>:CURIO_RELEASE>
									$<$<CONFIG:MinSizeRel>:CURIO_MINREL>
								PRIVATE
									CU_PLATFORM_SDL=1
							)

target_precompile_headers(curiolib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Private/CuPCH.h)

# IDEs should put the headers in a nice place
source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}/Public"
	PREFIX "Header Files"
	FILES ${PUBLIC_HEADER_LIST})
