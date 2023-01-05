# minimum cmake version
cmake_minimum_required(VERSION 3.9)

# project name
project(accountable-confirmer)

# flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(SOURCE_FILES main.cpp
        src/accountable_confirmer.cpp
        src/accountable_confirmer_bls.cpp
        src/tcp_server.cpp src/tcp_client.cpp src/tcp_connection.cpp)

# include
include_directories(include)
include_directories(include/bls)
include_directories(include/mcl)
include_directories(include/rb)

find_package(Boost 1.40 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(OPENSSL_ROOT_DIR /usr/local/opt/openssl@1.1)
set(OPENSSL_USE_STATIC_LIBS TRUE)

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(accountable-confirmer PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(accountable-confirmer "${CMAKE_THREAD_LIBS_INIT}")
endif()

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
#add_executable(accountable-confirmer
#        main.cpp
#        src/accountable_confirmer.cpp
#        src/accountable_confirmer_bls.cpp
#        src/socket.cpp src/network.cpp src/network_client.cpp)

#add_executable(accountable-confirmer src/network_server.cpp src/tcp_connection.cpp src/tcp_server.cpp)
#add_executable(accountable-confirmer_client src/network_client.cpp src/tcp_client.cpp)

add_executable(accountable-confirmer ${SOURCE_FILES})


find_library(BLS bls384_256 lib)
target_link_libraries(accountable-confirmer ${BLS})

#find_library(RB reliable-broadcast lib)
#target_link_libraries(accountable-confirmer ${RB})