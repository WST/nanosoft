cmake_minimum_required(VERSION 2.8)

project(ns_core)

add_definitions(-DUNIX)

set(NS_CORE_SRC base64.cpp doublepool.cpp fstream.cpp http.cpp mysql.cpp functions.c object.cpp simplepool.cpp nanostring.cpp error.cpp hash.cpp mutex.cpp nanoini.c nanourl.c options.cpp socket.cpp)
set(NS_CORE_HEADERS base64.h doublepool.h stream.h fstream.h http.h mysql.h functions.h object.h simplepool.h nanostring.h error.h hash.h mutex.h nanoini.h nanourl.h options.h socket.h)

add_library(ns_core STATIC ${NS_CORE_SRC})

INSTALL(TARGETS ns_core
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

INSTALL(FILES ${NS_CORE_HEADERS} DESTINATION include/nanosoft)
