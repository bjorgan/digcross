#convenience macro for adding new tests using the QtTest framework
#modified from https://wiki.qt.io/Unit_Testing
MACRO (add_qt_test testname testsrc)
	SET (test_${testname}_SRCS ${testsrc})
	set(CMAKE_AUTOMOC ON)
	find_package(Qt5Test)
	find_package(Qt5DBus)
	find_package(Qt5Widgets)

	add_executable(test_${testname} ${test_${testname}_SRCS} ../src/daemon_client.cpp ../src/cardreader.cpp)
	target_link_libraries(test_${testname} Qt5::Test Qt5::DBus Qt5::Widgets)
	ADD_TEST(test_${testname} test_${testname})
ENDMACRO (add_qt_test)

add_qt_test(daemonclient-t daemon_client-t.cpp)
add_qt_test(cardreader-t cardreader-t.cpp)
