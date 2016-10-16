#convenience macro for adding new tests using the QtTest framework
#modified from https://wiki.qt.io/Unit_Testing
MACRO (add_qt_test testname testsrc)
	SET (test_${testname}_SRCS ${testsrc})
	set(CMAKE_AUTOMOC ON)
	find_package(Qt5Test)
	find_package(Qt5DBus)
	find_package(Qt5Widgets)

	include_directories(../daemon)

	add_executable(test_${testname} ${test_${testname}_SRCS})
	target_link_libraries(test_${testname} Qt5::Test Qt5::DBus Qt5::Widgets digcross_test_common)
	ADD_TEST(test_${testname} test_${testname})
ENDMACRO (add_qt_test)

add_library(digcross_test_common ../src/daemon_client.cpp ../src/cardreader.cpp ../src/shoppinglist.cpp)
target_link_libraries(digcross_test_common Qt5::Test Qt5::DBus Qt5::Widgets)

add_qt_test(daemonclient daemon_client-t.cpp)
add_qt_test(cardreader cardreader-t.cpp)
add_qt_test(shoppinglist shoppinglist-t.cpp)
