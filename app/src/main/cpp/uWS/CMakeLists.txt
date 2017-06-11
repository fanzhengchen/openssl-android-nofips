cmake_minimum_required(VERSION 3.4.1)

project(WebSockets)
include_directories(${WebSockets_SOURCE_DIR}/../include/openssl)
include_directories(${WebSockets_SOURCE_DIR}/../include/libuv)


#GLOB or GLOB RECURSE


set(lib_dir ${WebSockets_SOURCE_DIR}/../../jniLibs/${ANDROID_ABI})
MESSAGE(STATUS "cmake source dir is " ${CMAKE_SOURCE_DIR})

add_library(crypto STATIC IMPORTED)
add_library(ssl STATIC IMPORTED)
add_library(uv STATIC IMPORTED)
add_library(decrepit STATIC IMPORTED)
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${lib_dir}/libcrypto.a)
set_target_properties(ssl PROPERTIES IMPORTED_LOCATION ${lib_dir}/libssl.a)
set_target_properties(uv PROPERTIES IMPORTED_LOCATION ${lib_dir}/libuv.a)
set_target_properties(decrepit PROPERTIES IMPORTED_LOCATION ${lib_dir}/libdecrepit.a)
set(SOURCES
    Networking.cpp WebSocket.cpp
    Socket.cpp Node.cpp
    Hub.cpp HTTPSocket.cpp
    Extensions.cpp Group.cpp)

add_library(uWS SHARED ${SOURCES})
target_link_libraries(uWS crypto ssl uv z)