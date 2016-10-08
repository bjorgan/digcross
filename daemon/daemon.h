#include <QObject>
#include <QDBusReply>

/**
 * Daemon object. Sets up dbus connection through system bus and exports processTransaction() as a DBus remote call.
 **/
class Daemon : public QObject {
	Q_OBJECT
	public:
		Daemon(QObject *parent = NULL);
	public slots:
		/**
		 * Receive request for transaction from GUI application.
		 * Returns with card number, current balance and transaction
		 * status when finished. (QString, QString, int)
		 *
		 * \param card_number Card number
		 * \param amount Amount
		 * \param msg Convenience argument for obtaining access to the reply (automatically assigned by DBus)
		 **/
		Q_SCRIPTABLE void processTransaction(QString card_number, QString amount, const QDBusMessage &msg);
};
