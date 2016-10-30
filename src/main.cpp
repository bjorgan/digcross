#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

	QApplication app(argc, argv);

	MainWindow mainWindow;
	mainWindow.show();

	app.exec();
}
