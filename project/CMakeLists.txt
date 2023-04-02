cmake_minimum_required(VERSION 3.15)

project(project)

include_directories(include ../vxworks/include)

add_executable(project src/main.c src/vector.c)

target_link_libraries(project vxworks)
