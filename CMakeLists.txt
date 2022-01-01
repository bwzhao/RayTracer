cmake_minimum_required(VERSION 3.16)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        )

file(GLOB_RECURSE SOURCE_FILES
        src/*.cpp
        )

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(RayTracer main.cpp include/ray.h)
target_include_directories(RayTracer PUBLIC ${INCLUDE_DIRECTORIES})
