#include "daemon_client-t.h"
#include <QSignalSpy>
#include "daemon_client.h"
#include <iostream>

const int SIGNAL_TIMEOUT = 1000;

void DaemonClientTest::daemonConnection()
{
	DaemonClient client;

	QSignalSpy transactionSpy(&client, SIGNAL(transactionFeedback(QString, float, TransactionStatus)));
	QVERIFY(transactionSpy.isValid());

	//send dummy card number
	QString cardNumber = "test_cardnumber_1234";
	client.processTransaction(cardNumber, 0);

	//verify that we receive the same card number back in the transaction feedback
	QVERIFY(transactionSpy.wait(SIGNAL_TIMEOUT));
	QCOMPARE(transactionSpy.count(), 1);
	QList<QVariant> arguments = transactionSpy.takeFirst();
	QCOMPARE(arguments.at(0).toString(), cardNumber);
}

QTEST_MAIN(DaemonClientTest)
