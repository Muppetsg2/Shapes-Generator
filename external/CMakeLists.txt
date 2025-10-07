# CPM - package manager
include(get_cpm)

# Wymuszenie budowania wszystkich bibliotek jako statycznych
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# glm https://github.com/g-truc/glm
CPMAddPackage("gh:g-truc/glm#1.0.1")
# fmt https://github.com/fmtlib/fmt
CPMAddPackage("gh:fmtlib/fmt#12.0.0")

set_target_properties(fmt glm PROPERTIES FOLDER "external")