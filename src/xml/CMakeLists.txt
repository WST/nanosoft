cmake_minimum_required(VERSION 2.8)

project(ns_xml)
set(NS_XML_SRC xmlnscontext.cpp  xmlnsmanager.cpp  xmlparser.cpp  xmlwriter.cpp)

include_directories(../core)

add_library(ns_xml STATIC ${NS_XML_SRC})
