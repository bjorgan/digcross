#include <QWidget>
#include "daemon_client.h"

class ShoppingList;

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
	private slots:
		void triggerTransaction(QString cardNumber);
		void receiveTransactionFeedback(QString cardNumber, float newBalance, DaemonClient::TransactionStatus status);
};
