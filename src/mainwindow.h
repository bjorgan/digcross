#include <QWidget>
#include "daemon_client.h"

class ShoppingList;
class CardReader;

/**
 * Mainwindow container widget opened when starting digcross.
 **/
class MainWindow : public QWidget {
	Q_OBJECT
	public:
		MainWindow(QWidget *parent = NULL);
	private:
		///List over items the user wants to buy
		ShoppingList *shoppingList;
		///Client interface against transaction daemon
		DaemonClient *transactionDaemon;
		///Card reader
		CardReader *cardReader;
	private slots:
		/**
		 * Start a transaction. Calculates amount to transact from shoppingList.
		 *
		 * \param cardNumber Card number
		 **/
		void triggerTransaction(QString cardNumber);

		/**
		 * Receive transaction feedback signal from DaemonClient.
		 *
		 * \param cardNumber Cardnumber for which transaction was attempted processed
		 * \param newBalance New balance of user
		 * \param status Transaction status
		 **/
		void receiveTransactionFeedback(QString cardNumber, float newBalance, DaemonClient::TransactionStatus status);

		/**
		 * Show message in MainWindow. Displays appropriate icons based on the success variable.
		 *
		 * \param success Whether action calling this function was a success or not
		 * \param message Message to display
		 **/
		void showMessage(bool success, QString message);

		/**
		 * Add a couple of test items to the shopping list.
		 **/
		void addTestItems();
};
