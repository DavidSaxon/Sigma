cmake_minimum_required(VERSION 2.6)
project(Sigma)

set(CMAKE_AUTOMOC ON)

set(CORE_LIB_SRC
    src/cxx/sigma/core/Sigma.cpp
    src/cxx/sigma/core/tasks/TasksDomain.cpp
    src/cxx/sigma/core/tasks/RootTask.cpp
    src/cxx/sigma/core/tasks/Task.cpp
)

set(UI_SRC
    src/cxx/sigma/Main.cpp
)

set(GUI_LIB_SRC
    src/cxx/sigma/gui/GUIBootstrap.cpp
)

set(TEST_SRC
    tests/cxx/TestsMain.cpp
    tests/cxx/core/Callback_TestSuite.cpp
    tests/cxx/core/task/TaskDomain_TestSuite.cpp
    tests/cxx/core/task/Task_TestSuite.cpp
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
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
        ${CMAKE_BINARY_DIR}/ext/ChaosCore/lib/linux_x86
)

add_library(sigma_core SHARED ${CORE_LIB_SRC})

target_link_libraries(sigma_core
    chaoscore_base
    chaoscore_io
)

add_library(sigma_gui SHARED ${GUI_LIB_SRC})

target_link_libraries(sigma_gui
    sigma_core
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

add_executable(Sigma ${UI_SRC})

target_link_libraries(Sigma
    sigma_gui
)

add_executable(tests ${TEST_SRC})

target_link_libraries(tests
    chaoscore_test
    sigma_core
)
