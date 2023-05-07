# add_library defines a new library target
# INTERFACE means, no source files (cpp files) and there will be no output for this library
add_library(rapidjson INTERFACE)

# JSONIncludeDir is a CMake variable
# CMAKE_CURRENT_SOURCE_DIR represents current score
# PARENT_SCOPE makes this available for the outside
# We can make use of RajidJson by including ${JSONIncludeDir}
set(JSONIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)