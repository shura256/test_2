cmake_minimum_required(VERSION 3.10)

project(test)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wfatal-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g1 -O0 -ggdb3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer")

add_executable(
  test
  main.cpp
)
