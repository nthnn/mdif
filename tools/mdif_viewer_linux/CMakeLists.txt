cmake_minimum_required(VERSION 3.5)

project(mdif_viewer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

include_directories(../../src)
add_executable(mdif_viewer mdif_viewer_main.cpp ../../src/mdif.cpp)
target_link_libraries(mdif_viewer Qt5::Core Qt5::Gui Qt5::Widgets)
