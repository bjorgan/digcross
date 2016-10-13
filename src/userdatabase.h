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
	userDatabase(const QString& path);

	/**
	 * @brief Destructor
	 * Close the db connection
	 */
	~userDatabase();

	/**
	 * @brief Find username associated to card
	 * @param CARD_ID - The card to investigate
	 * @return "USER_ID" - username for user associated to card
	 * 	   "CARD_NOT_FOUND" - error message for non-associated cards
	 */
	QString findUser(const QString& CARD_ID);

	/**
	 * @brief Create new row in database, populate with USER_ID
	 * @param USER_ID - username of new user
	 * @return 0 - SUCCESS - user successfully created
	 *         1 - USER_EXISTS_ERROR
	 */
	int createUser(const QString& USER_ID);

	/**
	 * @brief Delete user
	 * @param USER_ID - username to delete
	 * @return 0 - SUCCESS - User successfully deleted
	 *  	   1 - USER_EXISTS_ERROR
	 */
	int deleteUser(const QString& USER_ID);
	/**
	 * @brief Edit card information for user
	 * @param USER_ID - username to change card info for
	 * 	  CARD_ID - card info
	 * @return 0 - SUCCESS - Card id for user successfully updated
	 *         1 - USER_EXISTS_ERROR
	 *
	 */
	int editCard(QString USER_ID, QString CARD_ID);
private:
	QSqlDatabase m_db;
	
	/**
	 * @brief Check if user exists in db
	 * @param USER_ID - username to check
	 * @return true - person exists, false - person does not exist
	 */
	bool usernameExists(const QString& USER_ID);        

	/**
	 * @brief Check if card exists in db
	 * @param CARD_ID - card to check
	 * @return true - card exists, false - card does not exist
	 */
	bool cardExists(const QString& CARD_ID);		
}
