#include "mainwindow.h"
#include "cardreader.h"
#include "shoppinglist.h"

#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QTimer>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
{
	QVBoxLayout *layout = new QVBoxLayout(this);

	QPushButton *addTestItems = new QPushButton(tr("Add test items"));
	connect(addTestItems, SIGNAL(clicked()), SLOT(addTestItems()));
	layout->addWidget(addTestItems);

	//status message widget
	statusBar = new StatusBar;
	layout->addWidget(statusBar);

	//install cardReader as an filter which intercepts _all_ keyboard events to the application
	cardReader = new CardReader(this);
	connect(cardReader, SIGNAL(newCardNumber(QString)), SLOT(triggerTransaction(QString)));

	//shoppinglist and associated view
	shoppingList = new ShoppingList(this);

	ShoppingListWidget *shoppingListWidget = new ShoppingListWidget(shoppingList);
	layout->addWidget(shoppingListWidget);

	//daemon client
	transactionDaemon = new DaemonClient(this);
	connect(transactionDaemon, SIGNAL(transactionFeedback(QString, float, DaemonClient::TransactionStatus)), SLOT(receiveTransactionFeedback(QString, float, DaemonClient::TransactionStatus)));
}

void MainWindow::addTestItems()
{
	shoppingList->newItem("Brus", 15, 2);
	shoppingList->newItem("Godterisopp", 2, 35);
	shoppingList->newItem("Kvikklunsj", 10, 1);
	shoppingList->newItem("Ragni spesial", 788.5, 3);
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
	if (status == DaemonClient::TRANSACTION_SUCCESSFUL) {
		statusBar->showMessage(tr("Transaction processed. New balance: kr ") + QString::number(newBalance), StatusBar::SUCCESS_ICON);
		shoppingList->wipeList();
	} else {
		statusBar->showMessage(DaemonClient::errorMessage(status), StatusBar::ERROR_ICON);
	}
	this->setEnabled(true);
}

StatusBar::StatusBar(QObject *parent)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	text = new QLabel;
	icon = new QLabel;
	timer = new QTimer(this);

	layout->addWidget(icon);
	layout->addWidget(text, Qt::AlignLeft);

	connect(timer, SIGNAL(timeout()), SLOT(clearMessage()));
}

void StatusBar::showMessage(QString message, StatusIcon icon, int timeout)
{
	timer->stop();

	//status text
	text->setText(message.simplified());

	//status icon
	QSize iconSize = text->size();
	switch (icon) {
		case ERROR_ICON:
			this->icon->setPixmap(qApp->style()->standardIcon(QStyle::SP_MessageBoxCritical).pixmap(iconSize));
			break;
		case SUCCESS_ICON:
			this->icon->setPixmap(qApp->style()->standardIcon(QStyle::SP_DialogApplyButton).pixmap(iconSize));
			break;
	}

	timer->start(timeout);
}

void StatusBar::clearMessage()
{
	text->clear();
	icon->clear();
	timer->stop();
}
