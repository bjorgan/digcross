#include <QtTest/QTest>

class DaemonClientTest : public QObject {
	Q_OBJECT
	private slots:
		/**
		 * Test that DBus service provided by the digcrossd daemon is available.
		 **/
		void dbusServiceAvailable();

		/**
		 * Connect to transaction daemon and verify the feedback.  For
		 * convenience in checking that the daemon and full dbus chain
		 * is correctly set up (kind of integration test, will assume
		 * that the daemon and associated configs are correctly
		 * installed to system-wide paths).
		 **/
		void sendTransactionRequestToDaemonAndWaitForFeedback();
};
