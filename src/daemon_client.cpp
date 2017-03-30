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
	TransactionStatus status;
	QString username;
	double balance;

	QDBusPendingReply<QString, QString, int> reply = call->reply();

	if (!reply.isValid()) {
		username = UNKNOWN_USER;
		balance = UNKNOWN_BALANCE;
		status = DBUS_ERROR;
	} else {
		//parse returned values and emit result
		username = reply.argumentAt(0).toString();
		balance = reply.argumentAt(1).toString().toFloat();
		status = (TransactionStatus)reply.argumentAt(2).toInt();
	}

	emit transactionFeedback(username, balance, status);

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
			return QString(tr("Card was not found in local database."));
		case UFS_UNAVAILABLE:
			return QString(tr("Could not contact ufs."));
		case DBUS_ERROR:
			return QString(tr("Could not contact transaction daemon."));
	}
}
