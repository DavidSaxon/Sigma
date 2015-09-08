cmake_minimum_required(VERSION 2.6)
project(Sigma)

set(SRCS

    src/prototyping/QtTest.cpp

    src/sigma_core/tasks/TasksDomain.cpp
)

set(BASEPATH "${CMAKE_SOURCE_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x")

include_directories("${BASEPATH}" ${INCLUDE_DIRECTORIES} src/)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

link_directories(${LINK_DIRECTORIES})

add_executable(Sigma ${SRCS})

target_link_libraries(Sigma
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)
