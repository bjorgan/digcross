#include "daemon_client.h"
#include "../daemon/daemon_common.h"
#include <QDBusInterface>
#include <QDBusReply>
#include <cstdio>
#include <iostream>

DaemonClient::DaemonClient(QObject *parent) : QObject(parent)
{
	qRegisterMetaType<DaemonClient::TransactionStatus>("DaemonClient::TransactionStatus");
}

void DaemonClient::processTransaction(QString card_number, float amount)
{
	QDBusInterface iface(DBUS_SERVICE_NAME, "/", "", QDBusConnection::systemBus());

	//send message to daemon and prepare for reply
	QDBusPendingCall pendingCall = iface.asyncCall("processTransaction", card_number, QString::number(amount));
	QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pendingCall, this);
	connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), SLOT(transactionFinished(QDBusPendingCallWatcher*)));
}

void DaemonClient::transactionFinished(QDBusPendingCallWatcher *call)
{
	QDBusPendingReply<QString, QString, int> reply = call->reply();

	if (!reply.isValid()) {
		emit dbusError(reply.error().message());
		return;
	}

	//parse returned values and emit result
	QString username = reply.argumentAt(0).toString();
	QString balance = reply.argumentAt(1).toString();
	TransactionStatus status = (TransactionStatus)reply.argumentAt(2).toInt();

	emit transactionFeedback(username, balance.toFloat(), status);

	call->deleteLater();
}

QString DaemonClient::errorMessage(TransactionStatus status)
{
	switch (status) {
		case TRANSACTION_SUCCESSFUL:
			return QString();
		case USER_BLACKLISTED:
			return QString(tr("User is blacklisted."));
		case USER_NOT_IN_UFS_DATABASE:
			return QString(tr("User was not found in ufs database."));
		case USER_NOT_IN_LOCAL_DATABASE:
			return QString(tr("Card number was not found in local database."));
		case UFS_UNAVAILABLE:
			return QString(tr("Could not contact ufs."));
	}
}
