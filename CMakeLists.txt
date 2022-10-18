# minimum cmake version
cmake_minimum_required(VERSION 3.9)

# project name
project(accountable-confirmer)

# flags
set(CMAKE_CXX_STANDARD 17)


# executable
add_executable(accountable-confirmer src/bls.cpp main.cpp)

# target
target_include_directories(accountable-confirmer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_library(BLS libbls384_256 lib)
target_link_libraries(accountable-confirmer LINK_PUBLIC ${BLS})
