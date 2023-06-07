cmake_minimum_required(VERSION 3.22)

add_executable(MinecraftTool MinecraftTool.cpp)

# PUBLIC , include directories will be made available to any target that has links
# against MinecraftTool target
# PROJECT_BINARY_DIR: Cmake variable that represents the binary outut dir of the current project
# JSONIncludeDir: user defined variable (check rapidjson/CMakeLists.txt)
target_include_directories(MinecraftTool PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${CommonCodeIncludeDir}"
    )

target_link_libraries(MinecraftTool PUBLIC CommonCode)
