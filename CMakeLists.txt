cmake_minimum_required(VERSION 3.16)

project(Registerer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Widgets)

# Turns on AUTOMOC/AUTOUIC/AUTORCC and the usual output layout.
qt_standard_project_setup()

qt_add_executable(Registerer WIN32
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  bitformat.h
)

target_link_libraries(Registerer PRIVATE Qt6::Widgets)

install(
  TARGETS Registerer
  RUNTIME DESTINATION bin
)

# Tests for the Qt-free display logic; needs no Qt and no display.
enable_testing()
find_package(GTest REQUIRED)
add_executable(bitformat_test bitformat_test.cpp)
target_link_libraries(bitformat_test PRIVATE GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(bitformat_test)
