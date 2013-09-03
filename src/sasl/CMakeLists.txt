cmake_minimum_required(VERSION 2.8)

project(ns_sasl)
set(NS_SASL_SRC saslserver.cpp gsaslserver.cpp)

add_library(ns_sasl STATIC ${NS_SASL_SRC})
