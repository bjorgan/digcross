#include <QObject>
#include <QString>

/**
 * Client interface against daemon dealing with ufs connection/authentication.
 * Ufs transactions are sent through this client.
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

		/**
		 * Connect to daemon.
		 **/
		DaemonClient(QObject *parent);
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
		 * \param username Username username connected
		 * \param newBalance New balance for user after transaction
		 * \param status Status of transaction
		 **/
		void transactionFeedback(QString username, float newBalance, TransactionStatus status);
};
