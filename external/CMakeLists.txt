# CPM - package manager
include(get_cpm)

# Wymuszenie budowania wszystkich bibliotek jako statycznych
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# glm https://github.com/g-truc/glm
CPMAddPackage("gh:g-truc/glm#1.0.1")
# fmt https://github.com/fmtlib/fmt
CPMAddPackage("gh:fmtlib/fmt#12.0.0")
# Catch2 https://github.com/catchorg/Catch2
CPMAddPackage("gh:catchorg/Catch2@3.11.0")

set(Catch2_Extras ${Catch2_SOURCE_DIR}/extras CACHE INTERNAL "")

set_target_properties(Catch2WithMain fmt glm PROPERTIES FOLDER "external")