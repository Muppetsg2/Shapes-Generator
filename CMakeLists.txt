cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(global_settings)
include(utils)

shapes_generator_extract_version()

project(Shapes-Generator VERSION ${SHAPES_GENERATOR_VERSION} LANGUAGES CXX)
message(STATUS "Build Shapes-Generator: ${SHAPES_GENERATOR_VERSION}")

# ---- Dependencies ----
add_subdirectory(thirdparty)

# ---- Resource's files ----
add_subdirectory(res)

# ---- Main project's files ----
add_subdirectory(src)

if(WIN32)
	set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
elseif(APPLE)
	set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
	set_target_properties(${PROJECT_NAME} PROPERTIES 
		MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
		MACOSX_BUNDLE_BUNDLE_VERSION "${SHAPES_GENERATOR_VERSION}"
		RESOURCE "${ASSETS_FILES}"
	)
endif()