cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(global_settings)
include(utils)

get_version()

project(Shapes-Generator VERSION ${APP_VERSION} LANGUAGES CXX)
message(STATUS "Build ${PROJECT_NAME}: ${APP_VERSION}")

# ---- Dependencies ----
add_subdirectory(external)

# ---- Resource's files ----
add_subdirectory(res)

# ---- Main project's files ----
add_subdirectory(src)

# ---- Tests ----
add_subdirectory(tests)

set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR})

if(WIN32)
	set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${OUTPUT_DIR}")
elseif(APPLE)
	set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
	set_target_properties(${PROJECT_NAME} PROPERTIES 
		MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
		MACOSX_BUNDLE_BUNDLE_VERSION "${SHAPES_GENERATOR_VERSION}"
		RESOURCE "${ASSETS_FILES}"
	)
endif()