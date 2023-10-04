cmake_minimum_required(VERSION 3.14)
project(object-oriented-chess-game)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
set(EXECUTABLE_OUTPUT_PATH "../bin")

add_executable(main ${SOURCES})