#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

class userDatabase
{
public:
	/**
	 * @brief Constructor
	 * Constructor sets up connection with db and opens it
	 * @param path - absolute path to db file
	 */
	userDatabase(QString path);

	/**
	 * @brief Destructor
	 * Close the db connection
	 */
	~userDatabase();
	/**
	 * @brief Create new row in database, populate with USER_ID
	 * @param USER_ID - username of new user
	 * @return 0 - SUCCESS - user successfully created
	 *         1 - USER_EXISTS_ERROR - user already exists
	 */
	int createUser(QString username);

	/**
	 * @brief Delete user
	 * @param USER_ID - username to delete
	 * @return 0 - SUCCESS - User successfully deleted
	 *  	   2 - NO_USER_ERROR - user does not exist
	 */
	int deleteUser(QString username);
	/**
	 * @brief Edit card information for user
	 * @param USER_ID - username to change card info for
	 * 	  CARD_ID - card info
	 * @return 0 - SUCCESS - Card id for user successfully updated
	 *         2 - NO_USER_ERROR - user does not exist
	 *         3 - CARD_OWNED_ERROR - card associated with different user
	 */
	int editCard(QString username, QString card_id);

	/**
	 * @brief Find username associated to card
	 * @param CARD_ID - The card to investigate
	 *        result - Pointer which will be populated with username if SUCCESS
	 * @return 0 - SUCCESS - username associated with card written to result
	 * 	   4 - NO_USER_ERROR - No users associated with card, result populated with "error"
	 */
	int findUser(const QString& card_id, const QString *result);

private:
	QSqlDatabase m_db;
	
	/**
	 * @brief Check if user exists in db
	 * @param USER_ID - username to check
	 * @return true - person exists, false - person does not exist
	 */
	bool usernameExists(QString username);        

	/**
	 * @brief Check if card exists in db
	 * @param CARD_ID - card to check
	 * @return true - card exists, false - card does not exist
	 */
	bool cardExists( QString card_id);		
}
