#include <QObject>
#include <QtTest/QTest>

class UserDatabase;

class UserDatabaseTest : public QObject 
{
	Q_OBJECT
	private slots:
		/*g
		 * Create/clean up database, populate database with default items. init and cleanup		    * are called respecively before and after each test.
		 **/
		void init();
		void cleanup();
		
		/**
		 * Check that users are created successfully
		 **/
		void testUserAdd();

		/**
		 * Check that users are deleted successfully, also check that card data is not
		 * retained for deleted user
		 **/
		void testUserDelete();

		/**
		 * Check that card number returns correct user
		 **/
		void testCardMatchesUser();

		/**
		 * Check that existing card information can be modified, check that owned cards are not multiply assigned
		 **/
		void testModifyCardInfo();
}
