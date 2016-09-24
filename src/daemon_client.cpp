#include "daemon_client.h"
		
DaemonClient::DaemonClient(QObject *parent) : QObject(parent)
{
}

void DaemonClient::processTransaction(QString card_number, float amount)
{
}
