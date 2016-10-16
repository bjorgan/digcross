#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include "userdatabase.h"

//Requires that a compatible database exists, create one by:
//Starting sqlite3:     "sqlite3 digcross.db"
//Initializing the table: 
//"CREATE TABLE people(
//	USER_ID TEXT PRIMARY KEY NOT NULL,
//	CARD_ID TEXT);   
//(Optional) Verify database has been created successfully by calling: ".tables"
//Quit sqlite3: 	".quit"
//Tables can be deleted by calling: "DROP TABLE TABLE_NAME;"
//
//The PRIMARY KEY keyword indicates that this variable is the default for a column
//
//For use in actual application the database will have to be fetched by a cronjob
//or similar tools from its backup location. 



UserDatabase::UserDatabase(QString path)
{
	m_db = QSqlDatabase::addDatabase("QSQLITE");
	m_db.setDatabaseName(path);

	if (!m_db.open()
	{
		qDebug() << "Error: connection with database failed";
	}
	else
	{
		qDebug() << "Database: connection ok";
	}

}

UserDatabase::~UserDatabase()
{
	if (m_db.isOpen())
		{
			m_db.close();
		}
}

bool UserDatabase::usernameExists(QString username)
{
	bool not_exists = false;

	QSqlQuery query;
	query.prepare("SELECT name FROM people WHERE name = (:name)");
	query.bindValue(":name", name);

	if(query.exec())
	{
		if(query.next())
		{
			not_exists = true;
		}
	}
	else
	{
		qDebug() << "person exists failed: " << query.lastError();
	}
	return not_exists;
}

bool UserDatabase::cardExists(QString card_id)
{


}

bool UserDatabase::addUsername(QString username)
{
	bool success = false;
	//Verify that username is real and part of pool. Do query against ufs for
	//verification, delete otherwise. Do this here or in higher level?
	QSqlQuery query;
	query.prepare("INSERT INTO people (name) VALUES (:name)"); // Prepare query
	query.bindValue(":name", name); //Fill placeholders with proper values
	if(query.exec()) //Execute query
	{
		success = true;
	}
	else
	{
		qDebug() << "addUsername error: "
			 << query.lastError();
	}
	return success;
}


