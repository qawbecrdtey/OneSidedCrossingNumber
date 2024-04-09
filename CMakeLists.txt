cmake_minimum_required(VERSION 3.27)
project(OneSidedCrossingNumber)

option(NO_COMMENTS "Assume there are no comments in the input file. Slight improvement in performance." OFF)

add_executable(OneSidedCrossingNumber oscm_exact.cpp)
set_target_properties(OneSidedCrossingNumber PROPERTIES CXX_STANDARD 23)
target_include_directories(OneSidedCrossingNumber PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(OneSidedCrossingNumber PRIVATE -Wall -Wextra)

if(CMAKE_BUILD_TYPE EQUAL "Release")
    target_compile_options(OneSidedCrossingNumber PRIVATE -O2 -DNDEBUG)
endif()

if(NO_COMMENTS)
    target_compile_definitions(OneSidedCrossingNumber PRIVATE NO_COMMENTS)
endif()
