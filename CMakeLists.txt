cmake_minimum_required(VERSION 3.16)
project(RayTracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        )

file(GLOB_RECURSE SOURCE_FILES
        main.cpp
        src/*.cpp
        )

set(SOURCE_FILES_PI pi.cpp)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(RayTracer ${SOURCE_FILES})
target_include_directories(RayTracer PUBLIC ${INCLUDE_DIRECTORIES})

add_executable(Pi ${SOURCE_FILES_PI})
target_include_directories(Pi PUBLIC ${INCLUDE_DIRECTORIES})
