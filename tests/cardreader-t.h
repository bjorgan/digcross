#include <QtTest/QTest>
#include "cardreader.h"
#include <QWidget>

class CardReaderTest : public QObject {
	Q_OBJECT
	private slots:
		/**
		 * Checks that we get the expected result when a simulated RFID reader inputs characters.
		 **/
		void simulateRFIDReader();

		/**
		 * Simulates a slow keyboard user and verifies that input characters are ignored.
		 **/
		void simulateKeyboardInput();

		/**
		 * Simulate a second RFID read.
		 **/
		void simulateSecondRFIDReader();

		/**
		 * Set up CardReader and install as event filter for a widget.
		 **/
		void initTestCase();
	private:
		CardReader cardReader;
		QWidget widget;
};
