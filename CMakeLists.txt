cmake_minimum_required(VERSION 3.16)
project(RayTracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        )

file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        )

set(SOURCE_FILES_MC_Test MC_test.cpp)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(RayTracer main.cpp ${SOURCE_FILES})
target_include_directories(RayTracer PUBLIC ${INCLUDE_DIRECTORIES})

add_executable(MCTest ${SOURCE_FILES_MC_Test})
target_include_directories(MCTest PUBLIC ${INCLUDE_DIRECTORIES})
