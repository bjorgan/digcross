#include "cardreader-t.h"
#include <QSignalSpy>

const QString TEST_CARDNUMBER = "1234_testcard";

void CardReaderTest::initTestCase()
{
	widget.installEventFilter(&cardReader);
}

void CardReaderTest::simulateRFIDReader()
{
	QSignalSpy spy(&cardReader, SIGNAL(newCardNumber(QString)));
	QVERIFY(spy.isValid());

	QTest::keyClicks(&widget, TEST_CARDNUMBER);
	QTest::keyClick(&widget, Qt::Key_Return);

	QCOMPARE(spy.count(), 1);

	QList<QVariant> arguments = spy.takeFirst();
	QCOMPARE(arguments.at(0).toString(), TEST_CARDNUMBER);
}

void CardReaderTest::simulateKeyboardInput()
{
	QSignalSpy spy(&cardReader, SIGNAL(newCardNumber(QString)));
	QVERIFY(spy.isValid());

	int keyTypingTime = CARDREADER_TIME_LIMIT_MS*1.0/((TEST_CARDNUMBER.size())*1.0);
	QTest::keyClicks(&widget, TEST_CARDNUMBER, Qt::NoModifier, keyTypingTime);
	QTest::keyClick(&widget, Qt::Key_Return);

	QCOMPARE(spy.count(), 0);
}

void CardReaderTest::simulateSecondRFIDReader()
{
	simulateRFIDReader();
}

QTEST_MAIN(CardReaderTest)
