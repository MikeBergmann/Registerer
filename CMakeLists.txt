cmake_minimum_required(VERSION 3.5.1)

project(Registerer)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc and uic automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Widgets)

# Tell CMake to create the helloworld executable
add_executable(Registerer WIN32 mainwindow.ui mainwindow.cpp main.cpp)

# Use the Widgets module from Qt 5.
target_link_libraries(Registerer Qt5::Widgets)
