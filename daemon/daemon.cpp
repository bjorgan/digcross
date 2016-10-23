#include "daemon.h"
#include <QtCore/QTimer>
#include <QtDBus/QtDBus>
#include <iostream>
#include "daemon_common.h"
#include <syslog.h>

void Daemon::processTransaction(QString card_number, QString amount, const QDBusMessage &msg)
{
	//TODO: Lookup username from card_number.

	//TODO: Load authkey from file.

	//TODO: Do real calls against backend API.

	//TODO: Get result from transaction. Build feedback reply.

	//prepare dummy reply: nickname, balance, transaction status
	QList<QVariant> return_values;
	return_values << QVariant(card_number);
	return_values << QVariant(QString::number(-600));
	return_values << QVariant(2);
	QDBusMessage reply = msg.createReply(return_values);
	QDBusConnection::systemBus().send(reply);
}

void write_to_syslog(const char *message)
{
	openlog("digcrossd", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
	syslog(LOG_ERR, message);
	closelog();
}

Daemon::Daemon(QObject *parent) : QObject(parent)
{
	//check for dbus errors
	if (!QDBusConnection::systemBus().isConnected()) {
		write_to_syslog("Cannot connect to the D-Bus system bus.");
		exit(1);
	}

	if (!QDBusConnection::systemBus().registerService(DBUS_SERVICE_NAME)) {
		write_to_syslog(qPrintable(QDBusConnection::systemBus().lastError().message()));
		exit(1);
	}

	//set up dbus connection across system bus
	QDBusConnection::systemBus().registerObject("/", this, QDBusConnection::ExportScriptableSlots);
}

