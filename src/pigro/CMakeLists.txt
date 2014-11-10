cmake_minimum_required(VERSION 2.8)

project(pigro)

add_executable(pigro pigro.c)


INSTALL(TARGETS pigro RUNTIME DESTINATION bin)
