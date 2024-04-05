cmake_minimum_required(VERSION 3.27)
project(OneSidedCrossingNumber)

find_package(Boost REQUIRED)

set(OSCM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/oscm/segment_tree.cpp
        src/oscm/count_crossings.cpp)

add_executable(OneSidedCrossingNumber oscm.cpp ${OSCM_SRC})
set_target_properties(OneSidedCrossingNumber PROPERTIES CXX_STANDARD 20)
target_link_libraries(OneSidedCrossingNumber PRIVATE ${Boost_LIBRARIES})
target_include_directories(OneSidedCrossingNumber PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})
