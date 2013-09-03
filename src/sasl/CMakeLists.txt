cmake_minimum_required(VERSION 2.8)

project(ns_sasl)

set(NS_SASL_SRC saslserver.cpp gsaslserver.cpp)
set(NS_SASL_HEADERS saslserver.h gsaslserver.h)

add_definitions(-DHAVE_GSASL)

add_library(ns_sasl STATIC ${NS_SASL_SRC})

INSTALL(TARGETS ns_sasl
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

INSTALL(FILES ${NS_SASL_HEADERS} DESTINATION include/nanosoft)
