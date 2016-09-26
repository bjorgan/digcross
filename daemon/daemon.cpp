#include "daemon.h"
#include <QtCore/QTimer>
#include <QtDBus/QtDBus>
#include <iostream>

void Daemon::processTransaction(QString card_number, QString amount)
{
	std::cout << "Received transaction: " << card_number.toStdString() << " " << amount.toStdString() << std::endl;
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
	QDBusConnection::systemBus().registerObject("/", this, QDBusConnection::ExportAllSlots);
}

