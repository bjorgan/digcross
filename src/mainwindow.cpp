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
	cardReader = new CardReader(this);
	connect(cardReader, SIGNAL(newCardNumber(QString)), SLOT(triggerTransaction(QString)));
	enableCardReader(true);

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

#include <QMessageBox>

void MainWindow::receiveTransactionFeedback(QString cardNumber, float newBalance, DaemonClient::TransactionStatus status)
{
	if (status == DaemonClient::TRANSACTION_SUCCESSFUL) {
		showMessage(true, "Transaction processed, balance is now " + QString::number(newBalance));
	} else {
		showMessage(false, DaemonClient::errorMessage(status));
	}
	this->setEnabled(true);
}

void MainWindow::showMessage(bool success, QString message)
{
	enableCardReader(false);

	//FIXME: This should display the result in the MainWindow widget itself
	//instead of a separate message box.
	QString title = tr("Transaction status");
	if (!success) {
		QMessageBox::critical(this, title, message);
	} else {
		QMessageBox::information(this, title, message);
	}

	enableCardReader(true);
}

void MainWindow::enableCardReader(bool on)
{
	if (on) {
		qApp->installEventFilter(cardReader);
	} else {
		qApp->removeEventFilter(cardReader);
	}
}
