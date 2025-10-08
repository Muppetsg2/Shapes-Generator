# ---- Core lib's files ----
add_subdirectory(core)

# Precompiled header
set(PCH_FILE ${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp)

set(SUBPROJECT_NAME ${PROJECT_NAME})
set(SOURCE_FILES main.cpp)
set(HEADER_FILES ConsoleTemplates.hpp)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Resources"    FILES ${ASSETS_FILES})

# Define the executable
if(APPLE)
	add_executable(${SUBPROJECT_NAME} MACOSX_BUNDLE ${SOURCE_FILES} ${HEADER_FILES} ${ASSETS_FILES})
else()
	add_executable(${SUBPROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${ASSETS_FILES})
endif()

# Precompiled headers
target_precompile_headers(${SUBPROJECT_NAME} PUBLIC ${PCH_FILE})

set_target_properties(${SUBPROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(MSVC)
    # Antivirusdetect this program, which is statically linked with the MSVC runtime, as a virus.
    # set_property(TARGET ${SUBPROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
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

target_link_libraries(${SUBPROJECT_NAME} PUBLIC ${PROJECT_NAME}CoreLib fmt::fmt)

if(APPLE)
    target_link_libraries(${SUBPROJECT_NAME} PUBLIC "-framework CoreFoundation")
endif()

if(WIN32)
	copy_dlls_to_target(${SUBPROJECT_NAME} ${DLL_DIRECTORY})
endif()