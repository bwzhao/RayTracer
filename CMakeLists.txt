cmake_minimum_required(VERSION 3.16)
project(RayTracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/tinyobjloader/
        )

file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        )

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(CornellBox examples/CornellBox.cpp ${SOURCE_FILES})
target_include_directories(CornellBox PUBLIC ${INCLUDE_DIRECTORIES})

add_executable(CornellBoxReverseLight examples/CornellBox_ReverseLight.cpp ${SOURCE_FILES})
target_include_directories(CornellBoxReverseLight PUBLIC ${INCLUDE_DIRECTORIES})

add_executable(TestCornellBoxReverseLight examples/Test_CornellBox_ReverseLight.cpp ${SOURCE_FILES})
target_include_directories(TestCornellBoxReverseLight PUBLIC ${INCLUDE_DIRECTORIES})
