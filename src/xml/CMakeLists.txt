cmake_minimum_required(VERSION 2.8)

project(ns_xml)

set(NS_XML_SRC xmlnscontext.cpp xmlnsmanager.cpp xmlparser.cpp xmlwriter.cpp)
set(NS_XML_HEADERS xmlnscontext.h xmlnsmanager.h xmlparser.h xmlwriter.h)

include_directories(../core)

add_library(ns_xml STATIC ${NS_XML_SRC})

INSTALL(TARGETS ns_xml
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

INSTALL(FILES ${NS_XML_HEADERS} DESTINATION include/nanosoft)
