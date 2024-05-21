cmake_minimum_required(VERSION 3.27)
project(OneSidedCrossingNumber)

option(NO_COMMENTS "Assume there are no comments in the input file. Slight improvement in performance during input phase." OFF)

add_executable(OneSidedCrossingNumber oscm_exact.cpp)
set_target_properties(OneSidedCrossingNumber PROPERTIES CXX_STANDARD 26)
target_include_directories(OneSidedCrossingNumber PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(OneSidedCrossingNumber PRIVATE -Wall -Wextra -pedantic)

if(CMAKE_BUILD_TYPE EQUAL "Release")
    target_compile_options(OneSidedCrossingNumber PRIVATE -O3 -DNDEBUG -march=native -ffast-math -fipa-pta -flto -fdevirtualize-at-ltrans)
endif()

if(NO_COMMENTS)
    target_compile_definitions(OneSidedCrossingNumber PRIVATE NO_COMMENTS)
endif()
