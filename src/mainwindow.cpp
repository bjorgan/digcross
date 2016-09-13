#include "mainwindow.h"
#include <QGridLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
{
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(new QLabel("ARK kryssesystem"));
}
