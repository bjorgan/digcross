#ifndef DAEMON_CLIENT_H_DEFINED
#define DAEMON_CLIENT_H_DEFINED

#include <QObject>
#include <QString>
#include <QDBusPendingCallWatcher>

/**
 * Client interface against daemon dealing with ufs connection/authentication.
 * Ufs transactions are sent through this client.
 *
 * - To process a transaction, call processTransaction() on a given card number and amount to process.
 * - To receive a result from this transaction, connect transactionFeedback() to a suitable SLOT.
 * - DBus-related errors are signaled through dbusError().
 **/
class DaemonClient : public QObject {
	Q_OBJECT
	public:
		/**
		 * Status of processed transaction.
		 **/
		enum TransactionStatus{TRANSACTION_SUCCESSFUL,
				       USER_BLACKLISTED,
				       USER_NOT_IN_UFS_DATABASE,
				       USER_NOT_IN_LOCAL_DATABASE,
				       UFS_UNAVAILABLE};

		DaemonClient(QObject *parent = NULL);
	public slots:
		/**
		 * Attempt to process transaction for a card number and amount.
		 *
		 * \param card_number Card number as read from the RFID card reader
		 * \param amount Amount to be processed
		 **/
		void processTransaction(QString card_number, float amount);
	signals:
		/**
		 * Signalled when result of last transaction is received from
		 * the daemon. By design, we restrict database information to
		 * the information defined here.
		 *
		 * \param username Username connected to card number
		 * \param newBalance New balance for user after transaction
		 * \param status Status of transaction
		 **/
		void transactionFeedback(QString username, float newBalance, TransactionStatus status);

		/**
		 * Signalled on DBus errors.
		 *
		 * \param errorMessage Error message
		 **/
		void dbusError(QString errorMessage);
	private slots:
		/**
		 * For catching internal signal when daemon is finished with
		 * transaction and a reply has been sent.  Will emit
		 * transactionFeedback() with the results.
		 *
		 * \param watcher DBus pending reply watcher
		 **/
		void transactionFinished(QDBusPendingCallWatcher *watcher);
};

#endif
