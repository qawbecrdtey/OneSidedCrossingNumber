cmake_minimum_required(VERSION 3.27)
project(OneSidedCrossingNumber)

option(NO_COMMENTS "Assume there are no comments in the input file. Slight improvement in performance during input phase. Defaults to OFF." OFF)
option(DEBUG_MESSAGE "Prints debugging message. Defaults to OFF." OFF)
option(ENABLE_TIMER "Enables internal timing for the main operation. Defaults to OFF." OFF)

add_executable(OneSidedCrossingNumber oscm_exact.cpp)
set_target_properties(OneSidedCrossingNumber PROPERTIES CXX_STANDARD 26)
target_include_directories(OneSidedCrossingNumber PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(OneSidedCrossingNumber PRIVATE -Wall -Wextra -pedantic)

message("${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(OneSidedCrossingNumber PRIVATE -O2 -DNDEBUG -march=native -ffast-math -fipa-pta -flto -fdevirtualize-at-ltrans -msse -msse2 -msse3 -mmmx -m3dnow)
endif()

if(NO_COMMENTS)
    target_compile_definitions(OneSidedCrossingNumber PRIVATE NO_COMMENTS)
endif()

if(DEBUG_MESSAGE)
    target_compile_definitions(OneSidedCrossingNumber PRIVATE DEBUG_MESSAGE)
endif()

if(ENABLE_TIMER)
    target_compile_definitions(OneSidedCrossingNumber PRIVATE ENABLE_TIMER)
endif()
