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

set(lib_dir ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
#add_library(uv STATIC IMPORTED)
#set_target_properties(uv PROPERTIES IMPORTED_LOCATION ${lib_dir}/libuv.a)


target_link_libraries( # Specifies the target library.
                       native-lib
                       uWS
                       log )