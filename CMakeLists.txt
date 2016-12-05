cmake_minimum_required(VERSION 2.8.11)
project(digikryss)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(Qt5DBus)

#main GUI application
add_executable(digcross src/main.cpp src/mainwindow.cpp src/cardreader.cpp src/daemon_client.cpp src/shoppinglist.cpp)
target_link_libraries(digcross Qt5::Widgets Qt5::DBus)

#transaction daemon
add_executable(digcrossd daemon/main.cpp daemon/daemon.cpp)
target_link_libraries(digcrossd Qt5::DBus Qt5::Widgets)

#daemon config
set(DIGCROSSD_USER "digcrossd")
configure_file(daemon-config/org.digcross.daemonconnection.service.in org.digcross.daemonconnection.service @ONLY)
configure_file(daemon-config/org.digcross.daemonconnection.conf.in org.digcross.daemonconnection.conf @ONLY)

#daemon installation
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.digcross.daemonconnection.service DESTINATION /usr/share/dbus-1/system-services)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.digcross.daemonconnection.conf DESTINATION /etc/dbus-1/system.d)
install(TARGETS digcrossd DESTINATION bin)

#daemon user creation
configure_file(${CMAKE_SOURCE_DIR}/CreateDigcrossdUser.cmake.in ${CMAKE_BINARY_DIR}/CreateDigcrossdUser.cmake @ONLY)
install(SCRIPT "${CMAKE_BINARY_DIR}/CreateDigcrossdUser.cmake")

#tests
if (WITH_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif (WITH_TESTING)
