# CPM - package manager
include(CPM)

# glm https://github.com/g-truc/glm
CPMAddPackage("gh:g-truc/glm#1.0.1")
# fmt https://github.com/fmtlib/fmt
CPMAddPackage("gh:fmtlib/fmt#11.2.0")

set_target_properties(fmt glm PROPERTIES FOLDER "thirdparty")