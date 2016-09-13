#include "mainwindow.h"
#include "cardreader.h"

#include <QGridLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
{
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(new QLabel("ARK kryssesystem. Bipp et kort:"));

	QLabel *testLabel = new QLabel;
	layout->addWidget(testLabel);

	//install cardReader as an filter which intercepts keyboard events
	CardReader *cardReader = new CardReader(this);
	installEventFilter(cardReader);
	connect(cardReader, SIGNAL(newCardNumber(QString)), testLabel, SLOT(setText(QString)));
}
