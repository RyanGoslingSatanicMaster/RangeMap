cmake_minimum_required(VERSION 3.25)
project(rangeMap VERSION 0.0.9 DESCRIPTION "RangeMap data structure")

set(CMAKE_CXX_STANDARD 20)

add_executable(
        ${PROJECT_NAME}
        main.cpp
        tests/DeleteTests.cpp
        tests/GetTests.cpp
        tests/InsertTests.cpp
        tests/GetTests.h
        tests/InsertTests.h
)

add_subdirectory(range_map)

target_include_directories(${PROJECT_NAME} INTERFACE range_map)

target_link_directories(${PROJECT_NAME} PRIVATE range_map)

target_link_libraries(${PROJECT_NAME} RangeMapLib)
