# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)



add_library( # Sets the name of the library.
             native-lib

             SHARED

             native-lib.cpp )
include_directories(${CMAKE_SOURCE_DIR}/openssl)
include_directories(${CMAKE_SOURCE_DIR}/libuv)


find_library( # Sets the name of the path variable.
              log-lib
              log )

add_subdirectory(uWS)

file(GLOB_RECURSE Head "*.h")

target_link_libraries( # Specifies the target library.
                       native-lib
                       uWS
                       log )