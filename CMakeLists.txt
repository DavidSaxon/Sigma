cmake_minimum_required(VERSION 2.6)
project(Sigma)

set(CMAKE_AUTOMOC ON)

set(SRCS

    src/cxx/sigma/Main.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x")

include_directories(
        ${INCLUDE_DIRECTORIES}
        ${CMAKE_BINARY_DIR}/ext/ChaosCore/include/
        src/cxx
)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

link_directories(
        ${LINK_DIRECTORIES}
        ${CMAKE_BINARY_DIR}/ext/ChaosCore/lib
)

add_executable(Sigma ${SRCS})

target_link_libraries(Sigma
    chaoscore_base
    chaoscore_io
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)
