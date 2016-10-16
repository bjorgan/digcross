#include "mainwindow.h"
#include "cardreader.h"

#include <QGridLayout>
#include <QLabel>

#include "shoppinglist.h"

MainWindow::MainWindow(QWidget *parent)
{
	QGridLayout *layout = new QGridLayout(this);
	QLabel *testLabel = new QLabel;
	layout->addWidget(testLabel);

	//install cardReader as an filter which intercepts keyboard events
	CardReader *cardReader = new CardReader(this);
	installEventFilter(cardReader);
	connect(cardReader, SIGNAL(newCardNumber(QString)), testLabel, SLOT(setText(QString)));

	//shoppinglist and associated view
	ShoppingList *list = new ShoppingList(this);

	ShoppingListWidget *shoppingListWidget = new ShoppingListWidget(list);
	layout->addWidget(shoppingListWidget);

	//FIXME: Populate with some example data.
	list->newItem("Brus", 15, 2);
	list->newItem("Godterisopp", 2, 35);
	list->newItem("Kvikklunsj", 10, 1);
	list->newItem("Ragni spesial", 788.5, 3);
}
