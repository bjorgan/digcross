#include <QObject>

#define DBUS_SERVICE_NAME "org.digcross.daemonconnection"

class Daemon : public QObject {
	Q_OBJECT
	public:
		Daemon(QObject *parent = NULL);
	public slots:
		/**
		 * Receive request for transaction from GUI application.
		 *
		 * \param card_number Card number
		 * \param amount Amount
		 **/
		Q_SCRIPTABLE void processTransaction(QString card_number, QString amount);
};
