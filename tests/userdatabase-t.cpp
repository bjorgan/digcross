#include "userdatabase-t.h"



//Requires that a compatible database exists, create one by:
//Starting sqlite3:     "sqlite3 digcross.db"
//Initializing the table: 
//"CREATE TABLE people(
//	username TEXT PRIMARY KEY NOT NULL,
//	card_id TEXT);   
//(Optional) Verify database has been created successfully by calling: ".tables"
//Quit sqlite3: 	".quit"
//Tables can be deleted by calling: "DROP TABLE TABLE_NAME;"
//
//The PRIMARY KEY keyword indicates that this variable is the default for a column
//
//For use in actual application the database will have to be fetched by a cronjob
//or similar tools from its backup location. 


void UserDatabaseTest::init()
{
	//Fill empty database at test_data/test_database.db with initial values.
	m_db = QSqlDatabase::addDatabase("QSQLITE");
	m_db.setDatabaseName("test_data/test_database.db");

	m_db.open();

	QSqlQuery query;

	query.exec("CREATE TABLE people(
		username TEXT PRIMARY KEY NOT NULL,
		card_id TEXT);");

	query.exec("INSERT INTO people VALUES(oyvkar,123)");
	query.exec("INSERT INTO people VALUES(asgeirbj, 9001");
	query.exec("INSERT INTO people VALUES(stigvis, 007");
		
}

void UserDatabaseTest::cleanup()
{
	// Delete database content.
	QSqlQuery query;

	query.exec("DROP TABLE *"); //Should drop all tables, test this.
}

void UserDatabaseTest::testUserAdd()
{
	// Add a user and verify that it was added correctly.

}

void UserDatabaseTest::testUserDelete()
{
	// Verify that user and card data is deleted sucessfully.
}

void UserDatabaseTest::testCardMatchesUser()
{
	// Verify that card number returns correct user.
}

void UserDatabseTest::testModifyCardInfo()
{
	//Check that existing card information can be modified, check that owned card cannot be multiply assigned. 

}

QTEST_MAIN(UserDatabaseTest)

