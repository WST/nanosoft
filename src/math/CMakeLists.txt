cmake_minimum_required(VERSION 2.8)

project(ns_math)
set(NS_MATH_SRC math.cpp mathparser.cpp)
set(NS_MATH_HEADERS math.h mathparser.h)

add_library(ns_math STATIC ${NS_MATH_SRC})

INSTALL(TARGETS ns_math
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

INSTALL(FILES ${NS_MATH_HEADERS} DESTINATION include/nanosoft)
