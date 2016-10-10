#include <QtTest/QTest>

class DaemonClientTest : public QObject {
	Q_OBJECT
	private slots:
		/**
		 * Connect to transaction daemon and verify the feedback.  For
		 * convenience in checking that the daemon and full dbus chain
		 * is correctly set up (not really a proper unit test, will
		 * assume that the daemon and associated configs are correctly
		 * installed).
		 **/
		void daemonConnection();
};
