cmake_minimum_required(VERSION 2.8)
project(nanosoft)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -Wall")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -g -Wall")

add_subdirectory(src/core)
add_subdirectory(src/math)
add_subdirectory(src/netd)
add_subdirectory(src/xml)
add_subdirectory(src/sasl)

add_subdirectory(src/progs)
#add_subdirectory(src/examples)

ADD_CUSTOM_TARGET (distclean @echo cleaning for source distribution)
SET(DISTCLEANED CMakeCache.txt CMakeFiles cmake_install.cmake Makefile *.a)

ADD_CUSTOM_COMMAND (
	DEPENDS clean
	COMMENT "distribution clean"
	COMMAND rm
	ARGS -Rf ${DISTCLEANED}
	TARGET  distclean
)

INSTALL(FILES config.h DESTINATION include/nanosoft)
