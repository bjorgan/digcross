#include <QObject>
#include <QtTest/QTest>

class UserDatabase;

class UserDatabaseTest : public QObject 
{
	Q_OBJECT
	private slots:
		/**
		 * Create/clean up database, populate database with default items. init and cleanup		    * are called respecively before and after each test.
		 **/
		void init();
		void cleanup();
		
		/**
		 * Check that users are created successfully
		 **/
		void 
}
