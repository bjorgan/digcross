#include "daemon.h"
#include <QtCore/QTimer>
#include <QtDBus/QtDBus>

void UserGUIApplicationConnection::processTransaction(QString card_number, QString amount)
{
	QMetaObject::invokeMethod(QCoreApplication::instance(), "quit");
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

	UserGUIApplicationConnection *connection = new UserGUIApplicationConnection;
	QDBusConnection::systemBus().registerObject("/", connection, QDBusConnection::ExportAllSlots);
}

