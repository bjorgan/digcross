#include "mainwindow.h"
#include <QGridLayout>
#include <QLabel>
#include <QEvent>
#include <QKeyEvent>

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
{
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(new QLabel("ARK kryssesystem. Bipp et kort:"));

	QLabel *testLabel = new QLabel;
	layout->addWidget(testLabel);

	//install cardReader as an filter which intercepts keyboard events
	CardReader *cardReader = new CardReader(this);
	installEventFilter(cardReader);
	connect(cardReader, SIGNAL(newCardNumber(QString)), testLabel, SLOT(setText(QString)));
}

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
			emit newCardNumber(cardNumber);
		}

		cardNumber.clear();
	}

	return true;
}
