cmake_minimum_required(VERSION 3.15...4.0)
project(ruspberry VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib)

add_subdirectory(src)
