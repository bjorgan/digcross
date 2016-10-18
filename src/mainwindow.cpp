#include "mainwindow.h"
#include "cardreader.h"
#include "shoppinglist.h"

#include <QGridLayout>
#include <QLabel>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
{
	QGridLayout *layout = new QGridLayout(this);

	//install cardReader as an filter which intercepts _all_ keyboard events to the application
	CardReader *cardReader = new CardReader(this);
	qApp->installEventFilter(cardReader);
	connect(cardReader, SIGNAL(newCardNumber(QString)), SLOT(triggerTransaction(QString)));

	//shoppinglist and associated view
	shoppingList = new ShoppingList(this);

	ShoppingListWidget *shoppingListWidget = new ShoppingListWidget(shoppingList);
	layout->addWidget(shoppingListWidget);

	//Populate with some example data.
	//Menu class will have to connect to ShoppingList::newItem().
	shoppingList->newItem("Brus", 15, 2);
	shoppingList->newItem("Godterisopp", 2, 35);
	shoppingList->newItem("Kvikklunsj", 10, 1);
	shoppingList->newItem("Ragni spesial", 788.5, 3);

	//daemon client
	transactionDaemon = new DaemonClient(this);
	connect(transactionDaemon, SIGNAL(transactionFeedback(QString, float, DaemonClient::TransactionStatus)), SLOT(receiveTransactionFeedback(QString, float, DaemonClient::TransactionStatus)));
}

void MainWindow::triggerTransaction(QString cardNumber)
{
	if (shoppingList->numItems() > 0) {
		this->setEnabled(false);
		double totalPrice = shoppingList->getTotalPrice();
		transactionDaemon->processTransaction(cardNumber, totalPrice);
	}
}

void MainWindow::receiveTransactionFeedback(QString cardNumber, float newBalance, DaemonClient::TransactionStatus status)
{
	//this->setEnabled(true);
}
