cmake_minimum_required(VERSION 3.4.1)

PROJECT(WebSockets)
set(remote_include ${PROJECT_SOURCE_DIR}/../include)
include_directories(${remote_include})


#GLOB or GLOB RECURSE


set(lib_dir ${PROJECT_SOURCE_DIR}/../../jniLibs/${ANDROID_ABI})
MESSAGE(STATUS "cmake source dir is " ${CMAKE_SOURCE_DIR})

add_library(crypto SHARED IMPORTED)
add_library(ssl SHARED IMPORTED)
add_library(uv STATIC IMPORTED)
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${lib_dir}/libcrypto.so)
set_target_properties(ssl PROPERTIES IMPORTED_LOCATION ${lib_dir}/libssl.so)
set_target_properties(uv PROPERTIES IMPORTED_LOCATION ${lib_dir}/libuv.a)
#set(SOURCES
#    Networking.cpp WebSocket.cpp
#    Socket.cpp Node.cpp
#    Hub.cpp HTTPSocket.cpp
#    Extensions.cpp Group.cpp)
file(GLOB_RECURSE SOURCES "*.h" "*.cpp")

add_library(uWS SHARED ${SOURCES})
target_link_libraries(uWS crypto ssl uv z)