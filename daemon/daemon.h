#include <QObject>
#include <QDBusReply>

#define DBUS_SERVICE_NAME "org.digcross.daemonconnection"

/**
 * Daemon object. Sets up dbus connection and exports processTransaction() as a DBus remote call.
 **/
class Daemon : public QObject {
	Q_OBJECT
	public:
		Daemon(QObject *parent = NULL);
	public slots:
		/**
		 * Receive request for transaction from GUI application.
		 * Returns with name of user, current balance and transaction
		 * status when finished. (QString, QString, int)
		 *
		 * \param card_number Card number
		 * \param amount Amount
		 * \param msg Convenience argument for obtaining access to the reply (automatically assigned by DBus)
		 **/
		Q_SCRIPTABLE void processTransaction(QString card_number, QString amount, const QDBusMessage &msg);
};
