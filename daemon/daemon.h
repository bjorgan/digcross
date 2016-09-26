#include <QObject>

#define DBUS_SERVICE_NAME "org.digcross.daemonconnection"

/**
 * Server interface against which GUI application can communicate.
 **/
class UserGUIApplicationConnection : public QObject {
	Q_OBJECT
	public slots:
		/**
		 * Receive request for transaction from GUI application.
		 *
		 * \param card_number Card number
		 * \param amount Amount
		 **/
		Q_SCRIPTABLE void processTransaction(QString card_number, QString amount);
};

class Daemon : public QObject {
	Q_OBJECT
	public:
		Daemon(QObject *parent = NULL);
};
