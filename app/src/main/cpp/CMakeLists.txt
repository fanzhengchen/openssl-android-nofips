# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)
PROJECT(native)
include_directories(${PROJECT_SOURCE_DIR}/include)


add_library( # Sets the name of the library.
             native-lib

             SHARED

             native-lib.cpp )


find_library( # Sets the name of the path variable.
              log-lib
              log )

add_subdirectory(uWS)


target_link_libraries( # Specifies the target library.
                       native-lib
                       uWS
                       log )