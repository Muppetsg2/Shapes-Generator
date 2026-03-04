# CPM - package manager
include(get_cpm)

# Wymuszenie budowania wszystkich bibliotek jako statycznych
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Catch2 https://github.com/catchorg/Catch2
CPMAddPackage("gh:catchorg/Catch2@3.13.0")
if(MSVC AND Catch2_ADDED)
	set_property(TARGET Catch2 Catch2WithMain PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(Catch2_Extras ${Catch2_SOURCE_DIR}/extras CACHE INTERNAL "")

# fmt https://github.com/fmtlib/fmt
CPMAddPackage("gh:fmtlib/fmt#12.1.0")
if(MSVC AND fmt_ADDED)
	set_property(TARGET fmt-header-only PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# ftxui https://github.com/ArthurSonzogni/ftxui
CPMAddPackage(
	URI "gh:ArthurSonzogni/ftxui@6.1.9"
	OPTIONS "FTXUI_QUIET ON"
)
if(MSVC AND ftxui_ADDED)
	set_property(TARGET screen component dom PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# clip https://github.com/dacap/clip
CPMAddPackage(
	URI "gh:dacap/clip@1.15"
	OPTIONS "CLIP_INSTALL OFF"
			"CLIP_ENABLE_IMAGE OFF"
			"CLIP_EXAMPLES OFF"
			"CLIP_TESTS OFF"
)
if(MSVC AND clip_ADDED)
	set_property(TARGET clip PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# glm https://github.com/g-truc/glm
CPMAddPackage("gh:g-truc/glm#1.0.3")
if(MSVC AND glm_ADDED)
	set_property(TARGET glm-header-only PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# json https://github.com/nlohmann/json
CPMAddPackage("gh:nlohmann/json@3.12.0")

if(MSVC AND json_ADDED)
	set_property(TARGET nlohmann_json PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set_target_properties(Catch2WithMain # Catch2
					  fmt-header-only # fmt
					  glm-header-only # glm
					  component dom screen # ftxui
					  clip # clip
					  PROPERTIES FOLDER "external")