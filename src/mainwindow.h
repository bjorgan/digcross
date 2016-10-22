#include <QWidget>
#include "daemon_client.h"

class ShoppingList;
class CardReader;
class QStatusBar;
class QLabel;
class StatusBar;

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
		///Status bar for displaying messages to user
		StatusBar *statusBar;
	private slots:
		/**
		 * Start a transaction. Calculates amount to transact from shoppingList, sends to daemon client. Locks the GUI and the
		 * card reader.
		 *
		 * \param cardNumber Card number
		 **/
		void triggerTransaction(QString cardNumber);

		/**
		 * Receive transaction feedback signal from DaemonClient. Unlocks GUI and the card reader.
		 *
		 * \param username Username for which transaction was attempted processed
		 * \param newBalance New balance of user
		 * \param status Transaction status
		 **/
		void transactionFinished(QString username, float newBalance, DaemonClient::TransactionStatus status);

		/**
		 * Add a couple of test items to the shopping list.
		 *
		 * FIXME: Remove when main menu has been implemented.
		 **/
		void addTestItems();

		/**
		 * For receiving signal from shoppingList that the total price has changed. Informs the user through the status bar.
		 **/
		void updateDisplayPrice();
};

///Default timeout of status bar messages
const int STATUSBAR_TIMEOUT_MS = 5000;

/**
 * Status bar, displays messages to the user with an optional icon on the left side.
 * Can display temporary messages which revert to a persistent message after a given time.
 *
 * (Similar to QStatusBar, but with customized features.)
 **/
class StatusBar : public QWidget {
	Q_OBJECT
	public:
		StatusBar(QObject *parent = NULL);

		/**
		 * Get currently displayed status message.
		 *
		 * \return Current status message
		 **/
		QString currentMessage();

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
		 * Display temporary message on statusbar together with an optional icon.
		 * If a persistent message has been set, the status bar will revert to
		 * the persistent message after the timeout (otherwise set the status bar empty).
		 *
		 * \param message Message to display
		 * \param icon Icon to display
		 * \param timeout Timeout value of message
		 **/
		void setTemporaryMessage(QString message, StatusIcon icon = NO_ICON, int timeout = STATUSBAR_TIMEOUT_MS);

		/**
		 * Set a persistent message on the status bar. Any temporary messages will immediately
		 * be cleared. Temporary messages will overwrite the persistent message, but the status bar
		 * reverts to the persistent message after the timeout.
		 **/
		void setPersistentMessage(QString message);

		/**
		 * Clear temporary message from status bar, revert to persistent message (or empty status bar).
		 **/
		void clearTemporaryMessage();

		/**
		 * Clear persistent message from status bar, but not temporary messages that are still being displayed.
		 **/
		void clearPersistentMessage();
	private:
		///Message the status bar will revert to when the temporary message is cleared
		QString persistentMessage;
		///Status text
		QLabel *text;
		///Status icon
		QLabel *icon;
		///Timer for clearing temporary messages from the status bar
		QTimer *timer;

};
