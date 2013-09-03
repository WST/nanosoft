cmake_minimum_required(VERSION 2.8)

project(ns_netd)

set(NS_NETD_SRC asyncdns.cpp asyncobject.cpp asyncserver.cpp asyncstream.cpp asyncudpserver.cpp asyncxmlstream.cpp netdaemon.cpp)
set(NS_NETD_HEADERS asyncdns.h asyncobject.h asyncserver.h asyncstream.h asyncudpserver.h asyncxmlstream.h netdaemon.h)

find_library(ZLIB z)
#if(ZLIB)
#	add_definitions(-DFOUND_ZLIB)
#endif(ZLIB)

find_library(GNUTLS gnutls)
#if(GNUTLS)
#	add_definitions(-DFOUND_GNUTLS)
#endif(GNUTLS)

include_directories(../core)
include_directories(../xml) # for asyncxmlstream
include_directories(../..) # config.h

add_library(ns_netd STATIC ${NS_NETD_SRC})

INSTALL(TARGETS ns_netd
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

INSTALL(FILES ${NS_NETD_HEADERS} DESTINATION include/nanosoft)
