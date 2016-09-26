#include <stdio.h>
#include <stdlib.h>
#include <QtCore/QCoreApplication>
#include "daemon.h"

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);

	Daemon daemon;

	return app.exec();
}
