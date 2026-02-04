# CPM - package manager
include(get_cpm)

# Wymuszenie budowania wszystkich bibliotek jako statycznych
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# glm https://github.com/g-truc/glm
CPMAddPackage("gh:g-truc/glm#1.0.3")
if(MSVC)
	set_property(TARGET glm PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# fmt https://github.com/fmtlib/fmt
CPMAddPackage("gh:fmtlib/fmt#12.1.0")
if(MSVC)
	set_property(TARGET fmt-header-only PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# json https://github.com/nlohmann/json
CPMAddPackage("gh:nlohmann/json#v3.12.0")
if(MSVC)
	set_property(TARGET nlohmann_json PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Catch2 https://github.com/catchorg/Catch2
CPMAddPackage("gh:catchorg/Catch2@3.12.0")
if(MSVC)
	set_property(TARGET Catch2 Catch2WithMain PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(Catch2_Extras ${Catch2_SOURCE_DIR}/extras CACHE INTERNAL "")

set_target_properties(Catch2WithMain fmt-header-only glm PROPERTIES FOLDER "external")