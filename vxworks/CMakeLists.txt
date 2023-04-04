cmake_minimum_required(VERSION 3.15)

project(vxworks)

include_directories(include)

add_library(vxworks src/taskLib.c src/sysLib.c src/semLib.c src/helpers.c src/msgQLib.c src/wdLib.c src/tickLib.c)
