cmake_minimum_required(VERSION 2.8)

project(ns_netd)

set(NS_NETD_SRC asyncdns.cpp asyncobject.cpp asyncserver.cpp asyncstream.cpp asyncudpserver.cpp asyncxmlstream.cpp netdaemon.cpp)
set(NS_NETD_HEADERS asyncdns.h asyncobject.h asyncserver.h asyncstream.h asyncudpserver.h asyncxmlstream.h netdaemon.h)

add_definitions(-DFDBUFFER_BLOCK_SIZE=1024)
add_definitions(-DFD_READ_CHUNK_SIZE=4096)

find_library(ZLIB z)
if(ZLIB)
	add_definitions(-DHAVE_ZLIB)
endif(ZLIB)

find_library(GNUTLS gnutls)
if(GNUTLS)
	add_definitions(-DHAVE_GNUTLS)
endif(GNUTLS)

include_directories(../core)
include_directories(../xml) # for asyncxmlstream

add_library(ns_netd STATIC ${NS_NETD_SRC})

INSTALL(TARGETS ns_netd
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

INSTALL(FILES ${NS_NETD_HEADERS} DESTINATION include/nanosoft)
