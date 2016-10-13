#include "daemon_client-t.h"
#include <QSignalSpy>
#include "daemon_client.h"
#include <iostream>

#include <QtDBus/QtDBus>
#include "daemon_common.h"

const int SIGNAL_TIMEOUT = 1000;

void DaemonClientTest::dbusServiceAvailable()
{
	//check DBus daemon availability
	QDBusConnection connection = QDBusConnection::systemBus();
	QVERIFY(connection.isConnected());

	//check digcrossd service availability
	QDBusMessage call = QDBusMessage::createMethodCall(DBUS_SERVICE_NAME, QLatin1String("/"), QLatin1String("org.freedesktop.DBus.Introspectable"), QLatin1String("Introspect"));
	QDBusReply<QString> dbus_reply = connection.call(call);
	if (!dbus_reply.isValid()) {
		QDBusError err = dbus_reply.error();
		if (err.type() == QDBusError::ServiceUnknown) {
			QVERIFY2(dbus_reply.isValid(), "digcrossd .service file is missing");
		} else if (err.type() != QDBusError::Other) {
			QVERIFY2(dbus_reply.isValid(), qPrintable(err.message()));
		} else {
			QVERIFY2(dbus_reply.isValid(), QString("digcrossd could not be executed by DBus, insufficient DBus policies, missing executable or daemon exited with error code. DBus error message: " + err.message()).toStdString().c_str());
		}
	}
}

void DaemonClientTest::sendTransactionRequestToDaemonAndWaitForFeedback()
{
	DaemonClient client;

	QSignalSpy transactionSpy(&client, SIGNAL(transactionFeedback(QString, float, TransactionStatus)));
	QVERIFY(transactionSpy.isValid());

	//send dummy card number
	QString cardNumber = "test_cardnumber_1234";
	client.processTransaction(cardNumber, 0);

	//verify that we receive the same card number back in the transaction feedback
	QVERIFY2(transactionSpy.wait(SIGNAL_TIMEOUT), "DBus call timed out, digcrossd service likely not running.");
	QCOMPARE(transactionSpy.count(), 1);
	QList<QVariant> arguments = transactionSpy.takeFirst();
	QCOMPARE(arguments.at(0).toString(), cardNumber);
}

QTEST_MAIN(DaemonClientTest)
