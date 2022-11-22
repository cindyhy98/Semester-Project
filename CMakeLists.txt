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
#set(Boost_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/boost_1_80_0)
set(OPENSSL_ROOT_DIR /usr/local/opt/openssl@1.1)
set(OPENSSL_USE_STATIC_LIBS TRUE)
#set(OPENSSL_INCLUDE_DIR /usr/local/opt/openssl/lib/libssl)
#set(OPENSSL_CRYPTO_LIBRARY /usr/local/opt/openssl@1.1/lib/libcrypto)

#find_package(Boost)
#IF (Boost_FOUND)
#    include_directories(${Boost_INCLUDE_DIR})
#endif()


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

#include(ExternalProject)
include_directories(secp256k1/include)
#ExternalProject_Add(libsecp256k1
#        SOURCE_DIR secp256k1
#        CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/secp256k1/autogen.sh
#        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/secp256k1/configure --disable-shared --with-pic --with-bignum=no --enable-module-recovery
#        BUILD_COMMAND make
#        INSTALL_COMMAND ""
#        BUILD_IN_SOURCE 1)

add_library(secp256k1 STATIC IMPORTED)
set_target_properties(
        secp256k1
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/secp256k1/.libs/libsecp256k1.a)
add_dependencies(secp256k1 libsecp256k1)



# executable
add_executable(accountable-confirmer src/accountable_confirmer.cpp src/ac_bls.cpp src/ecdsa_pki.cpp
        src/message.cpp src/socket.cpp main.cpp)
#add_executable(accountable-confirmer-client src/ac.cpp src/ac_bls.cpp src/ecdsa_pki.cpp
#        src/message.cpp src/socket.cpp main_client.cpp)


target_link_libraries(accountable-confirmer OpenSSL::Crypto secp256k1)
#target_link_libraries(accountable-confirmer-client OpenSSL::Crypto secp256k1)

find_library(BLS bls384_256 lib)
target_link_libraries(accountable-confirmer ${BLS})
#target_link_libraries(accountable-confirmer-client ${BLS})