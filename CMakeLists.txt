# minimum cmake version
cmake_minimum_required(VERSION 3.9)

# project name
project(accountable-confirmer)

# flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# include
include_directories(include)
include_directories(include/bls)
include_directories(include/mcl)

# include boost -> Yet later need to change this setting to an universal one
set(Boost_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/boost_1_80_0)
set(OPENSSL_ROOT_DIR /usr/local/opt/openssl@1.1)
#set(OPENSSL_INCLUDE_DIR /usr/local/opt/openssl/lib/libssl)
#set(OPENSSL_CRYPTO_LIBRARY /usr/local/opt/openssl/lib/libcrypto)

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()




# executable
#add_executable(accountable-confirmer src/testbls.cpp main.cpp src/broadcast.cpp include/broadcast.h)
add_executable(accountable-confirmer src/generate_pki.cpp )


# Search OpenSSL
find_package(OpenSSL REQUIRED)
if( OpenSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIR})
    link_directories(${OPENSSL_LIBRARIES})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    message(STATUS "OpenSSL Not Found")
endif()

#target_link_libraries(accountable-confirmer ${OPENSSL_INCLUDE_DIR} ${OPENSSL_CRYPTO_LIBRARY})

find_library(BLS bls384_256 lib)
target_link_libraries(accountable-confirmer ${BLS})