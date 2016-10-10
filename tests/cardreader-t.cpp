#include "cardreader-t.h"

void CardReaderTest::initTestCase()
{
	widget.installEventFilter(&cardReader);
}

void CardReaderTest::simulateRFIDReader()
{
	QString testString = "1234_testcard";
	QTest::keyClicks(&widget, testString);
}

void CardReaderTest::simulateKeyboardInput()
{
}

QTEST_MAIN(CardReaderTest)
