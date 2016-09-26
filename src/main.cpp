#include <QApplication>
#include "mainwindow.h"
#include "daemon_client.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	//MainWindow mainWindow;
	//mainWindow.show();

	DaemonClient client;
	client.processTransaction("1234", 0);

	app.exec();
}
