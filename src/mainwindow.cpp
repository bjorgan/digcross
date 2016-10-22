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
	QGridLayout *layout = new QGridLayout(this);
	int col = 0;
	int row = 0;
	int num_cols = 2;

	//temporary test main menu. FIXME: Remove when main menu is implemented.
	QPushButton *addTestItems = new QPushButton(tr("Add test items"));
	connect(addTestItems, SIGNAL(clicked()), SLOT(addTestItems()));
	layout->addWidget(addTestItems, row, col, 1, num_cols);

	//install cardReader as an filter which intercepts _all_ keyboard events to the application
	cardReader = new CardReader(this);
	connect(cardReader, SIGNAL(newCardNumber(QString)), SLOT(triggerTransaction(QString)));

	//shoppinglist and associated view
	shoppingList = new ShoppingList(this);

	ShoppingListWidget *shoppingListWidget = new ShoppingListWidget(shoppingList);
	layout->addWidget(shoppingListWidget, ++row, col, 1, num_cols);
	connect(shoppingList, SIGNAL(totalPriceChanged()), SLOT(updateDisplayPrice()));

	//daemon client
	transactionDaemon = new DaemonClient(this);
	connect(transactionDaemon, SIGNAL(transactionFeedback(QString, float, DaemonClient::TransactionStatus)), SLOT(transactionFinished(QString, float, DaemonClient::TransactionStatus)));

	//status message widget
	statusBar = new StatusBar;
	layout->addWidget(statusBar, ++row, col, 1, num_cols);

	//wipe button for shopping list
	QPushButton *wipeButton = new QPushButton(qApp->style()->standardIcon(QStyle::SP_DialogResetButton), tr("Wipe list"));
	layout->addWidget(wipeButton, row, ++col, Qt::AlignRight);
	connect(wipeButton, SIGNAL(clicked()), shoppingList, SLOT(wipeList()));
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
		//disable input
		this->setEnabled(false);
		cardReader->disable();

		//send transaction
		double totalPrice = shoppingList->getTotalPrice();
		statusBar->setPermanentMessage(tr("Waiting for transaction..."));
		transactionDaemon->processTransaction(cardNumber, totalPrice);
	}
}

void MainWindow::transactionFinished(QString username, float newBalance, DaemonClient::TransactionStatus status)
{
	//display appropriate status messages for result of transaction
	if (status == DaemonClient::TRANSACTION_SUCCESSFUL) {
		double price = shoppingList->getTotalPrice();
		shoppingList->wipeList();
		statusBar->setPermanentMessage(tr("Last transaction: ") + QString::number(price) + " kr for " + username);
		statusBar->setTemporaryMessage(tr("Transaction processed. New balance for ") + username + ": kr " + QString::number(newBalance), StatusBar::SUCCESS_ICON);
	} else {
		updateDisplayPrice();
		statusBar->setTemporaryMessage(DaemonClient::errorMessage(status), StatusBar::ERROR_ICON);
	}

	//re-enable input
	this->setEnabled(true);
	cardReader->enable();
}

void MainWindow::updateDisplayPrice()
{
	double currPrice = shoppingList->getTotalPrice();

	if (currPrice > 0) {
		statusBar->setPermanentMessage(tr("Swipe a card to process the following sum: ") + QString::number(currPrice) + " kr");
	} else {
		statusBar->clearPermanentMessage();
	}
}

StatusBar::StatusBar(QObject *parent)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	text = new QLabel;
	icon = new QLabel;
	timer = new QTimer(this);

	layout->addWidget(icon);
	layout->addWidget(text, Qt::AlignLeft);

	connect(timer, SIGNAL(timeout()), SLOT(clearTemporaryMessage()));
}

void StatusBar::setTemporaryMessage(QString message, StatusIcon icon, int timeout)
{
	timer->stop();
	clearTemporaryMessage();

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
		default:
			break;
	}

	timer->start(timeout);
}

void StatusBar::clearTemporaryMessage()
{
	text->clear();
	icon->clear();
	timer->stop();

	//revert to permanent message
	if (permanentMessage.size() > 0) {
		text->setText(permanentMessage);
	}
}

void StatusBar::setPermanentMessage(QString message)
{
	permanentMessage = message;

	clearTemporaryMessage();
}

void StatusBar::clearPermanentMessage()
{
	permanentMessage = QString();

	if (!timer->isActive()) {
		clearTemporaryMessage();
	}
}
