cmake_minimum_required(VERSION 2.8.11)
project(digikryss)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(Qt5DBus)

#main GUI application
add_executable(digcross src/main.cpp src/mainwindow.cpp src/cardreader.cpp src/daemon_client.cpp src/shoppinglist.cpp)
target_link_libraries(digcross Qt5::Widgets Qt5::DBus)

#admin GUI
add_executable(digcross-admin admin-src/admin_main.cpp)
install(TARGETS digcross-admin DESTINATION bin)

#transaction daemon
add_executable(digcrossd daemon/main.cpp daemon/daemon.cpp)
target_link_libraries(digcrossd Qt5::DBus Qt5::Widgets)

#daemon config
set(DIGCROSSD_USER $ENV{USER}) #FIXME: Set a different username (e.g. digcrossd)
set(DAEMON_SERVICE_FILE "org.digcross.daemonconnection.service")
set(DAEMON_CONF_FILE "org.digcross.daemonconnection.conf")
configure_file(daemon-config/${DAEMON_SERVICE_FILE}.in ${DAEMON_SERVICE_FILE} @ONLY)
configure_file(daemon-config/${DAEMON_CONF_FILE}.in ${DAEMON_CONF_FILE} @ONLY)

#daemon installation
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DAEMON_SERVICE_FILE} DESTINATION /usr/share/dbus-1/system-services/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DAEMON_CONF_FILE} DESTINATION /etc/dbus-1/system.d/)
install(TARGETS digcrossd DESTINATION bin)

#polkit files
set(DIGCROSS_ADMIN_USER_GROUP "digcrossd_users")
set(AUTH_FILE "51-digcross-auth.conf")
set(POLKIT_ACTION "org.digcross.admin.authentication")
set(POLKIT_FILE "${POLKIT_ACTION}.policy")
configure_file(admin-config/${AUTH_FILE}.in ${AUTH_FILE} @ONLY)
configure_file(admin-config/${POLKIT_FILE}.in ${POLKIT_FILE} @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${AUTH_FILE} DESTINATION /etc/polkit-1/localauthority.conf.d/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${POLKIT_FILE} DESTINATION /usr/share/polkit-1/actions/)

#tests
if (WITH_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif (WITH_TESTING)
