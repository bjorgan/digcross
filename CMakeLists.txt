cmake_minimum_required(VERSION 2.8.11)
project(digikryss)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets)

add_executable(digikryss main.cpp)
target_link_libraries(digikryss Qt5::Widgets)
