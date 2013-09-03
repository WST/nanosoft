cmake_minimum_required(VERSION 2.8)

project(ns_math)
set(NS_MATH_SRC math.cpp mathparser.cpp)

add_library(ns_math STATIC ${NS_MATH_SRC})# Создание статической библиотеки
