cmake_minimum_required(VERSION 3.22)

# Set the name of the project
project(MinecraftToolProject)

# Add subdirecetory rapidjson to the build process
# Indicates there is a separate CMakeLists.txt fila present in the "rapidjson" directory
add_subdirectory(RapidJsonLib)
add_subdirectory(CommonCode)
add_subdirectory(MinecraftTool)

# Prints the value of ${PROJECT_SOURCE_DIR} CMake variable
# This variable represents the root directory of the current project
# STATUS means that the messge will be displayed during the CMake config process
message(STATUS "${PROJECT_SOURCE_DIR}")
message(STATUS "include directory: ${JSONIncludeDir}")

