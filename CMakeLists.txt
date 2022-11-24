# minimum cmake version
cmake_minimum_required(VERSION 3.9)

# project name
project(accountable-confirmer)

# flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/salticidae/cmake/Modules/")
#
#add_subdirectory(salticidae)
#include_directories(salticidae/include)

# include
include_directories(include)
include_directories(include/bls)
include_directories(include/mcl)


set(OPENSSL_ROOT_DIR /usr/local/opt/openssl@1.1)
set(OPENSSL_USE_STATIC_LIBS TRUE)

# Search OpenSSL
find_package(OpenSSL REQUIRED)
if( OpenSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIR})
#    include_directories(${OPENSSL_CRYPTO_LIBRARY})
#    link_directories(${OPENSSL_LIBRARIES})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    message(STATUS "OpenSSL Not Found")
endif()



# executable
add_executable(accountable-confirmer src/accountable_confirmer.cpp src/accountable_confirmer_bls.cpp
        src/socket.cpp main.cpp)




find_library(BLS bls384_256 lib)
target_link_libraries(accountable-confirmer ${BLS})