cmake_minimum_required(VERSION 3.10)

project(SmartCalc)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC "src/*.cpp")
include_directories(include)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME})