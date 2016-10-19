#include <QWidget>
#include "daemon_client.h"

class ShoppingList;
class CardReader;
class QStatusBar;
class QLabel;

///Default timeout of status bar messages
const int STATUSBAR_TIMEOUT_MS = 5000;

/**
 * Status bar. More or less like QStatusBar, but with an icon on the right side.
 **/
class StatusBar : public QWidget {
	Q_OBJECT
	public:
		StatusBar(QObject *parent = NULL);

		/**
		 * Icon to display in status bar.
		 **/
		enum StatusIcon {
			NO_ICON, //no icon
			ERROR_ICON, //display an error icon
			SUCCESS_ICON //display a green checkmark
		};
	public slots:
		/**
		 * Display message on statusbar together with an optional icon.
		 *
		 * \param message Message to display
		 * \param icon Icon to display
		 * \param timeout Timeout value of message
		 **/
		void showMessage(QString message, StatusIcon icon = NO_ICON, int timeout = STATUSBAR_TIMEOUT_MS);

		/**
		 * Clear message from status bar.
		 **/
		void clearMessage();
	private:
		///Status text
		QLabel *text;
		///Status icon
		QLabel *icon;
		///Timer for clearing the status bar
		QTimer *timer;

};

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
		StatusBar *statusBar;
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
		 * Add a couple of test items to the shopping list.
		 **/
		void addTestItems();
};
