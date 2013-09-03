cmake_minimum_required(VERSION 2.8)
project(nanosoft)

add_definitions(-DHAVE_GNUTLS)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall")

add_subdirectory(src/math)

