#include "daemon.h"
#include <QtCore/QTimer>
#include <QtDBus/QtDBus>
#include <iostream>
#include "daemon_common.h"
#include "daemon_client.h"
#include <unistd.h>

static double currentBalance = 400; //FIXME: Remove when real backend is implemented. Used for simulating a user account.

void Daemon::processTransaction(QString card_number, QString amount, const QDBusMessage &msg)
{
	//TODO: Lookup username from card_number.

	//TODO: Load authkey from file.

	//TODO: Do real calls against backend API.

	//TODO: Get result from transaction. Build feedback reply.
	sleep(1); //FIXME: Remove when real backend calls are implemented. This is for simulating a delay in the daemon.

	//prepare dummy reply
	QString username = UNKNOWN_USER;
	double newBalance = UNKNOWN_BALANCE;
	int transactionStatus = (int)DaemonClient::TRANSACTION_SUCCESSFUL;

	if (card_number == QString("1234")) { //"accept" transactions when card number is 1234
		username = "Ragno";
		if (currentBalance < 0) { //simulate that ufs has blacklisted the user
			newBalance = currentBalance;
			transactionStatus = (int)DaemonClient::USER_BLACKLISTED;
		} else {
			currentBalance -= amount.toDouble();
			newBalance = currentBalance;
		}
	} else {
		transactionStatus = (int)DaemonClient::USER_NOT_IN_LOCAL_DATABASE;
	}

	//construct reply: nickname, balance, transaction status
	QList<QVariant> return_values;
	return_values << QVariant(username) << QVariant(QString::number(newBalance)) << QVariant(transactionStatus);
	QDBusMessage reply = msg.createReply(return_values);
	QDBusConnection::systemBus().send(reply);
}

Daemon::Daemon(QObject *parent) : QObject(parent)
{
	//check for dbus errors
	if (!QDBusConnection::systemBus().isConnected()) {
		fprintf(stderr, "Cannot connect to the D-Bus system bus.\n");
		exit(1);
	}

	if (!QDBusConnection::systemBus().registerService(DBUS_SERVICE_NAME)) {
		fprintf(stderr, "%s\n",	qPrintable(QDBusConnection::systemBus().lastError().message()));
		exit(1);
	}

	//set up dbus connection across system bus
	QDBusConnection::systemBus().registerObject("/", this, QDBusConnection::ExportScriptableSlots);
}

