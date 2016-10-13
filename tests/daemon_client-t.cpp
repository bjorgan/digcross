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
			//likely error: .service-file was not installed to /usr/share/dbus-1/system.d/, run make install
			QVERIFY2(dbus_reply.isValid(), "digcrossd .service file is missing");
		} else {
			//likely error: .conf-file was not installed to /etc/dbus-1/system.conf, or digcrossd is missing from PATH, or digcrossd crashed on launch.
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
	QVERIFY(transactionSpy.wait(SIGNAL_TIMEOUT));
	QCOMPARE(transactionSpy.count(), 1);
	QList<QVariant> arguments = transactionSpy.takeFirst();
	QCOMPARE(arguments.at(0).toString(), cardNumber);
}

QTEST_MAIN(DaemonClientTest)
