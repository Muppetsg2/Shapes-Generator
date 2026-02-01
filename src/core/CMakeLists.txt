# Precompiled header
set(PCH_FILE ${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp)

# Add source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS *.hpp)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})

set(SUBPROJECT_NAME ${PROJECT_NAME}CoreLib)

# Define the library
add_library(${SUBPROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

# Precompiled headers
target_precompile_headers(${SUBPROJECT_NAME} PUBLIC ${PCH_FILE})

set_target_properties(${SUBPROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${SUBPROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    # Antivirus detect this program, which is statically linked with the MSVC runtime, as a virus.
    set_property(TARGET ${SUBPROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    target_compile_definitions(${SUBPROJECT_NAME} PUBLIC NOMINMAX)
endif()

target_compile_definitions(${SUBPROJECT_NAME} PRIVATE DEBUG="$<IF:$<CONFIG:Debug>,1,0>")
target_compile_definitions(${SUBPROJECT_NAME} PRIVATE SHAPES_GENERATOR_NAME="${PROJECT_NAME}")
target_compile_definitions(${SUBPROJECT_NAME} PRIVATE SHAPES_GENERATOR_VERSION="${APP_VERSION}")

if(WIN32)
    target_compile_definitions(${SUBPROJECT_NAME} PRIVATE _WIN32)
elseif(APPLE)
    target_compile_definitions(${SUBPROJECT_NAME} PRIVATE __APPLE__)
elseif(UNIX)
    target_compile_definitions(${SUBPROJECT_NAME} PRIVATE __linux__)
endif()    

# Optimizations
target_compile_options(${SUBPROJECT_NAME} PUBLIC
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
        $<$<CONFIG:Release>:/O2>
		$<$<CONFIG:Release>:/GL>
        $<$<CONFIG:Release>:/fp:fast>
        $<$<CONFIG:Debug>:/RTC1>
        /Zc:preprocessor
		/Zc:__cplusplus
    >
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-flto>
    >
    # disable noexcept type warning on GCC
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:-O3>
		$<$<CONFIG:Release>:-flto>
    >
)

target_link_options(${SUBPROJECT_NAME} PUBLIC
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
		$<$<CONFIG:Release>:/LTCG>
    >
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:-flto>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:-fuse-ld=lld>
        -static
    >
    # disable noexcept type warning on GCC
    $<$<CXX_COMPILER_ID:GNU>:
		$<$<CONFIG:Release>:-flto>
        -static
    >
)

target_link_libraries(${SUBPROJECT_NAME} PUBLIC glm::glm fmt::fmt-header-only)

if(APPLE)
    target_link_libraries(${SUBPROJECT_NAME} PUBLIC "-framework CoreFoundation")
endif()