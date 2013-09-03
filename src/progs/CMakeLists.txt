cmake_minimum_required(VERSION 2.8)

project(ns_progs)

add_executable(forker forker.c)
add_executable(wait wait.c)
add_executable(time time.c)
add_executable(uncat uncat.c)
add_executable(nsu nsu.c)
add_executable(showpath showpath.c)

# masterd
include_directories(../core)
add_executable(masterd masterd.c)
target_link_libraries(masterd ns_core)

# miniget
add_executable(miniget miniget.c)
target_link_libraries(miniget ns_core)

# miniget 2
add_executable(miniget2 miniget.cpp)
target_link_libraries(miniget2 ns_core)

# minisend
add_executable(minisend minisend.cpp)
target_link_libraries(minisend ns_core)