# minimum cmake version
cmake_minimum_required(VERSION 3.9)

# project name
project(accountable-confirmer)

# flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#include
include_directories(include)
include_directories(include/bls)
include_directories(include/mcl)


# executable
add_executable(accountable-confirmer src/testbls.cpp main.cpp)

# target
# target_include_directories(accountable-confirmer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_library(BLS bls384_256 lib)
target_link_libraries(accountable-confirmer LINK_PUBLIC ${BLS})
