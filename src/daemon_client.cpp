#include "daemon_client.h"
#include "../daemon/daemon.h"
#include <QDBusInterface>

DaemonClient::DaemonClient(QObject *parent) : QObject(parent)
{
}

void DaemonClient::processTransaction(QString card_number, float amount)
{
	QDBusInterface iface(DBUS_SERVICE_NAME, "/", "", QDBusConnection::systemBus());
	if (iface.isValid()) {
		iface.call("processTransaction", card_number, QString::number(amount));
		return;
	}
}
