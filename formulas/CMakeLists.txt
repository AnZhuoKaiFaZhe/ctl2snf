cmake_minimum_required(VERSION 3.17)
project(formulas)
set(CMAKE_CXX_STANDARD 14)

aux_source_directory(. SRC)
add_library(formulas ${SRC})