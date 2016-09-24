cmake_minimum_required(VERSION 2.8.11)
project(digikryss)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

add_executable(digikryss src/main.cpp src/mainwindow.cpp src/cardreader.cpp src/daemon_client.cpp)
target_link_libraries(digikryss Qt5::Widgets)
