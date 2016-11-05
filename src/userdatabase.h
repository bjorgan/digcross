#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

enum error_code
{
	SUCCESS = 0, 
	USER_EXISTS_ERROR = 1,
	NO_USER_ERROR = 2,
	CARD_OWNED_ERROR = 3,
	DATABASE_COMMUNICATION_ERROR = 4
};

class UserDatabase
{
public:
	/**
	 * Constructor sets up connection with db and opens it
	 * /param path - absolute path to db file
	 */
	UserDatabase(QString path);

	/**
	 * Destructor, closes the db connection
	 */
	~UserDatabase();
	/**
	 * Create new row in database, populate with USER_ID
	 * /param username - username of new user
	 *        card_id - optional parameter for card
	 * /return 0 - SUCCESS - user successfully created
	 *         1 - USER_EXISTS_ERROR - user already exists
	 *	   4 - DATABASE_COMMUNICATION_ERROR
	 */
	enum createUser(QString username, QString card_id = QString());

	/**
	 * Delete user
	 * /param username - username to delete
	 * /return 0 - SUCCESS - User successfully deleted
	 *  	   2 - NO_USER_ERROR - user does not exist
	 *	   4 - DATABASE_COMMUNICATION_ERROR
	 */
	enum deleteUser(QString username);
	/**
	 * Change card information for user
	 * /param username - username to change card info for
	 * 	  card_id - card info
	 * /return 0 - SUCCESS - Card id for user successfully updated
	 *         2 - NO_USER_ERROR - user does not exist
	 *         3 - CARD_OWNED_ERROR - card associated with different user
	 *	   4 - DATABASE_COMMUNICATION_ERROR
	 */
	enum changeCard(QString username, QString card_id);

	/**
	 * Find username associated to card
	 * /param card_id - The card to investigate
	 * 	  result - Pointer to result location
	 * /return 0 - SUCCESS - result populated with associated username
	 * 	   2 - NO_USER_ERROR - user does not exist
	 *	   4 - DATABASE_COMMUNICATION_ERROR
	 */
	enum findUser(QString card_id, QString *result);

private:
	QSqlDatabase m_db;
	
	/**
	 * Check if user exists in db
	 * /param username - username to check
	 * /return true - person exists, false - person does not exist
	 */
	bool usernameExists(QString username);        

	/**
	 * Check if card exists in db
	 * /param card_id - card to check
	 * /return true - card exists, false - card does not exist
	 */
	bool cardExists(QString card_id);		
}
