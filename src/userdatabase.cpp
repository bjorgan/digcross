#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include "userdatabase.h"

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
	bool exists = false;

	QSqlQuery query;
	query.prepare("SELECT username FROM people WHERE username = (:username)"); 
	query.bindValue(":username", username); //fill placeholders with actual values

	if(query.exec())
	{
		if(query.next())
		{
			exists = true;
		}
	}	
	return exists;
}

bool UserDatabase::cardExists(QString card_id)
{
	bool exists = false;

	QSqlQuery query;
	query.prepare("SELECT card_id FROM people WHERE card_id = (:card_id)"); //prepare query
	query.bindValue(":card_id", card_id); //fill placeholders with actual values

	if(query.exec())
	{
		if(query.next())
		{
			exists = true;
		}
	}	
	return exists;
}

int UserDatabase::addUsername(QString username)
{
	//Only call this function after user has been verified against ufs
	bool success = false;
	QSqlQuery query;
	query.prepare("INSERT INTO people (username) VALUES (:username)"); // Prepare query
	query.bindValue(":username", username); //Fill placeholders with proper values
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


