cmake_minimum_required(VERSION 3.13)
project(CYK)

set(CMAKE_CXX_STANDARD 17)

add_executable(CYK main.cpp ContextFreeGrammar.cpp ContextFreeGrammar.h)
