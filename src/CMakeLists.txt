# Precompiled header
set(PCH_FILE ${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp)

# Add source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS *.hpp)

source_group("Source Files" FILES ${SOURCE_FILES})						   
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Resources"    FILES ${ASSETS_FILES})

# Define the executable
if(APPLE)
	add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCE_FILES} ${HEADER_FILES} ${ASSETS_FILES})
else()
	add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${ASSETS_FILES})
endif()

# Precompiled headers
target_precompile_headers(${PROJECT_NAME} PUBLIC ${PCH_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Platform-specific definitions
if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NOMINMAX)
    target_compile_options(${PROJECT_NAME} PUBLIC "/Zc:preprocessor")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG="$<IF:$<CONFIG:Debug>,1,0>")

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32)
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE __APPLE__)
elseif(UNIX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE __linux__)
endif()

# Optimizations
target_compile_options(${PROJECT_NAME} PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:Debug>:/RTC1>
    >
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:-O2>
    >
    $<$<CXX_COMPILER_ID:Clang>:
        $<$<CONFIG:Release>:-O2>
    >
)

target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm fmt::fmt)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PUBLIC "-framework CoreFoundation")
endif()