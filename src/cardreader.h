#include <QObject>
#include <QElapsedTimer>

class QEvent;

///Allowed time for card reader to read input characters, in order to avoid normal keyboard input and only allow a fast RFID reader
const int CARDREADER_TIME_LIMIT_MS = 100;

/**
 * Receives input from RFID card reader. Accumulates characters in a buffer and
 * emits a signal when card number is ready. (Receives keyboard characters,
 * send signal on \n.) Functions as an event filter.
 **/
class CardReader : public QObject {
	Q_OBJECT
	public:
		CardReader(QObject *parent = NULL){};
	signals:
		/**
		 * New card number received from RFID reader.
		 *
		 * \param number Card number string
		 **/
		void newCardNumber(QString number);
	protected:
		/**
		 * Returns true and handles QKeyEvents. Emits characters accumulated when ENTER is pressed, given that the typing speed was lower than CARDREADER_TIME_LIMIT_MS.
		 **/
		bool eventFilter(QObject *object, QEvent *event);
	private:
		///Currently contained card number
		QString cardNumber;
		///Used for measuring time since first character
		QElapsedTimer writeTime;
};
