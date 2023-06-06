cmake_minimum_required(VERSION 3.22)

add_executable(MinecraftTool MinecraftTool.cpp)
target_link_libraries(MinecraftTool PUBLIC rapidjson)