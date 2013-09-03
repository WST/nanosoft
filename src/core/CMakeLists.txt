cmake_minimum_required(VERSION 2.8)

project(ns_core)

add_definitions(-DUNIX)

set(NS_CORE_SRC base64.cpp doublepool.cpp fstream.cpp http.cpp mysql.cpp functions.c object.cpp simplepool.cpp nanostring.cpp error.cpp hash.cpp mutex.cpp nanoini.c nanourl.c options.cpp socket.cpp)

add_library(ns_core STATIC ${NS_CORE_SRC})
