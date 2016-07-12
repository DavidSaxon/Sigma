cmake_minimum_required(VERSION 2.6)
project(Sigma)

set(CMAKE_AUTOMOC ON)

set(CORE_LIB_SRC
    src/cpp/sigma/core/Sigma.cpp
    src/cpp/sigma/core/tasks/TasksDomain.cpp
    src/cpp/sigma/core/tasks/RootTask.cpp
    src/cpp/sigma/core/tasks/Task.cpp
    src/cpp/sigma/core/util/Logging.cpp
)

set(META_QT_SRC
    src/cpp/meta_qt/core/Geometry.cpp
    src/cpp/meta_qt/core/Qt.cpp
)

set(GUI_LIB_SRC
    src/cpp/sigma/gui/GUIBootstrap.cpp
    src/cpp/sigma/gui/GUILogging.cpp
    src/cpp/sigma/gui/GUIMeta.cpp
    src/cpp/sigma/gui/GUIMetaCompiled.cpp
    src/cpp/sigma/gui/startup/SplashScreen.cpp
)

set(UI_SRC
    src/cpp/sigma/Main.cpp
)

set(TEST_SRC
    tests/cpp/TestsMain.cpp
    tests/cpp/core/Callback_TestSuite.cpp
    tests/cpp/core/task/TaskDomain_TestSuite.cpp
    tests/cpp/core/task/Task_TestSuite.cpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x")

include_directories(
        ${INCLUDE_DIRECTORIES}
        ${CMAKE_BINARY_DIR}/ext/ArcaneCore/include/
        ${CMAKE_BINARY_DIR}/ext/ArcaneLog/include/
        ${CMAKE_BINARY_DIR}/ext/MetaEngine/include/
        src/cpp
)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

link_directories(
        ${LINK_DIRECTORIES}
        ${CMAKE_BINARY_DIR}/ext/ArcaneCore/lib/linux_x86
        ${CMAKE_BINARY_DIR}/ext/ArcaneLog/lib/linux_x86
        ${CMAKE_BINARY_DIR}/ext/MetaEngine/lib/linux_x86
)

add_library(sigma_core STATIC ${CORE_LIB_SRC})

target_link_libraries(sigma_core
)

add_library(meta_qt STATIC ${META_QT_SRC})

target_link_libraries(meta_qt
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

add_library(sigma_gui STATIC ${GUI_LIB_SRC})

target_link_libraries(sigma_gui
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

add_executable(Sigma ${UI_SRC})

target_link_libraries(Sigma
    sigma_gui
    sigma_core
    meta_qt
    arcanelog_shared
    arcanelog
    metaengine
    arcanecore_io
    arcanecore_base
)

add_executable(tests ${TEST_SRC})

target_link_libraries(tests
    arcanecore_test
    arcanecore_io
    arcanecore_base
    sigma_core
)
