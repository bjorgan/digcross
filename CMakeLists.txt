cmake_minimum_required(VERSION 2.8.11)
project(digikryss)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(Qt5DBus)

add_executable(digcross src/main.cpp src/mainwindow.cpp src/cardreader.cpp src/daemon_client.cpp)
add_executable(digcrossd daemon/main.cpp daemon/daemon.cpp)
target_link_libraries(digcross Qt5::Widgets Qt5::DBus)
target_link_libraries(digcrossd Qt5::DBus Qt5::Widgets)

#dbus config
configure_file(daemon-config/org.digcross.daemonconnection.service.in org.digcross.daemonconnection.service @ONLY)
configure_file(daemon-config/org.digcross.daemonconnection.conf.in org.digcross.daemonconnection.conf @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.digcross.daemonconnection.service DESTINATION /usr/share/dbus-1/services)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.digcross.daemonconnection.conf DESTINATION /etc/dbus-1/system.d)
install(TARGETS digcrossd DESTINATION bin)
