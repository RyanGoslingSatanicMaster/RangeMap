cmake_minimum_required(VERSION 3.25)
project(rangeMap)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        rangeMap
        range_map/MutableRangeMap.h
        range_map/MutableRangeMap.cpp
        range_map/RangeNode.h
        range_map/MutableRangeNode.h
        range_map/exceptions/InvalidRangeException.h
        range_map/exceptions/InvalidRangeException.cpp
        range_map/MutableRangeNode.cpp
        range_map/RangeMap.h
        main.cpp
        range_map/RangeItem.h range_map/RangeItem.cpp range_map/RangeNode.cpp tests/GetTests.cpp)
