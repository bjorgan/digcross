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

	if (!m_db.open())
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

error_code UserDatabase::createUser(QString username, QString card_id = QString())
{
	if(usernameExists(username))
	{
		return USER_EXISTS_ERROR;
	}

	if(cardExists(card_id))
	{
		return CARD_EXISTS_ERROR;
	}

	QSqlQuery query;
	query.prepare("INSERT INTO people VALUES(:username,:card_id)"); // Prepare query
	query.bindValue(":username", username); //Fill placeholders with proper values
	query.bindValue(":card_id", card_id);
	if(query.exec()) //Execute query
	{
		return SUCCESS;
	}
	else
	{
		return DATABASE_COMMUNICATION_ERROR; 
	}
	return success;
}

error_code User_Database::findUser(QString card_id, QString *result)
{
	
	QSqlQuery query;
	query.prepare("select username from people where card_id = (:card_id)");
	query.bindValue(":card_id", card_id);
}

error_code User_Database::changeCard(QString username, QString card_id)
{
	query.prepare("update people set card_id = (:card_id) where username = (:username)");
	query.bindValue(":card_id", card_id);
	query.bindValue(":username", username);

}

error_code User_Database::deleteUser(QString username)
{
	query.prepare("delete from people where username = (:username)");
	query.bindValue(":username", username);

}
