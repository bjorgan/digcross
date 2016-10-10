#include <QtTest/QTest>

class DaemonClientTest : public QObject {
	Q_OBJECT
	private slots:
		/**
		 * Connect to transaction daemon and verify the feedback.  For
		 * convenience in checking that the daemon and full dbus chain
		 * is correctly set up (integration test).
		 **/
		void daemonConnection();
};
