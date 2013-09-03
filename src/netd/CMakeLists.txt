cmake_minimum_required(VERSION 2.8)

project(ns_netd)

set(NS_NETD_SRC asyncdns.cpp  asyncobject.cpp  asyncserver.cpp  asyncstream.cpp  asyncudpserver.cpp  asyncxmlstream.cpp  netdaemon.cpp)

add_definitions(-DFDBUFFER_BLOCK_SIZE=1024)
add_definitions(-DFD_READ_CHUNK_SIZE=4096)
add_definitions(-DHAVE_GNUTLS)

include_directories(../core)
include_directories(../xml)

add_library(ns_netd STATIC ${NS_NETD_SRC})# Создание статической библиотеки
