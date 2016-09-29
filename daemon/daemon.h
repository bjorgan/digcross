#include <QObject>
#include <QDBusReply>

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
		 * \param msg Convenience argument for obtaining access to the reply (automatically assigned by DBus)
		 **/
		Q_SCRIPTABLE void processTransaction(QString card_number, QString amount, const QDBusMessage &msg);
};
