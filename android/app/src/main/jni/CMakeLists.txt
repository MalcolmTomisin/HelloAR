cmake_minimum_required(VERSION 3.13)

# Set C++ Standard
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add specific compile options (like -Wall)
add_compile_options(-Wall)

# Define the library name here.
project(appmodules)

# Android system libraries
find_library(LOG_LIB log)
find_library(ANDROID_LIB android)
find_library(JNIGRAPHICS_LIB jnigraphics)
find_library(GLESV2_LIB GLESv2)
find_library(EGL_LIB EGL)

# This file includes all the necessary to let you build your React Native application
include(${REACT_ANDROID_DIR}/cmake-utils/ReactNative-application.cmake)

# Define where the additional source code lives. We need to crawl back the jni, main, src, app, android folders
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../../../../shared/NativeARModule.cpp)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ARViewNativeWrapper.cpp)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE utils.cc)

# Define where CMake can find the additional header files. We need to crawl back the jni, main, src, app, android folders
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ../../../../../shared)

# Import the ARCore library.
add_library(arcore SHARED IMPORTED)
set_target_properties(arcore PROPERTIES IMPORTED_LOCATION
              ${ARCORE_LIBPATH}/${ANDROID_ABI}/libarcore_sdk_c.so
              INTERFACE_INCLUDE_DIRECTORIES ${ARCORE_INCLUDE}
)

# Import the glm header file from the NDK.
add_library( glm INTERFACE )
set_target_properties( glm PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${GLM_INCLUDE})

target_link_libraries(${CMAKE_PROJECT_NAME}
        arcore
        glm
        ${LOG_LIB}
        ${ANDROID_LIB}
        ${JNIGRAPHICS_LIB}
        ${GLESV2_LIB}
        ${EGL_LIB}
)