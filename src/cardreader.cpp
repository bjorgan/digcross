#include "cardreader.h"

#include <QEvent>
#include <QKeyEvent>

bool CardReader::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() != QEvent::KeyPress) {
		return false;
	}

	if (cardNumber.isEmpty()) {
		writeTime.start();
	}

	//accumulate characters
	QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
	cardNumber += keyEvent->text();

	//send signal on enter (RFID card reader ends string input with an enter)
	if (keyEvent->key() == Qt::Key_Return) {
		//disallow entries with duration less than a time limit in order to avoid keyboard input and allow only the fast RFID reader
		if (writeTime.elapsed() < CARDREADER_TIME_LIMIT_MS) {
			emit newCardNumber(cardNumber.simplified());
		}

		cardNumber.clear();
	}

	return true;
}
