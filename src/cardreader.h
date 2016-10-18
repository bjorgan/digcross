#ifndef CARDREADER_H_DEFINED
#define CARDREADER_H_DEFINED

#include <QObject>
#include <QElapsedTimer>

class QEvent;

///Allowed time for card reader to read input characters, in order to avoid normal keyboard input and only allow a fast RFID reader
const int CARDREADER_TIME_LIMIT_MS = 100000;

/**
 * Receives input from RFID card reader. Accumulates characters in a buffer and
 * emits a signal when card number is ready. (Receives keyboard characters,
 * send signal on \n.) Blocks all keyboard input.
 *
 * Enable by using enable(), disable keyboard filtering using disable(). Enabled by default.
 **/
class CardReader : public QObject {
	Q_OBJECT
	public:
		CardReader(QObject *parent = NULL){enable();};

		/**
		 * Enable card reader input and block all keyboard input to all other widgets.
		 **/
		void enable();

		/**
		 * Disable card reader input and enable keyboard input to other widgets.
		 **/
		void disable();
	signals:
		/**
		 * New card number has been received from RFID reader.
		 *
		 * \param number Card number string
		 **/
		void newCardNumber(QString number);
	protected:
		/**
		 * Accumulates keypresses. Emits the accumulated characters through newCardNumber() when ENTER is pressed, given that the typing speed was lower than CARDREADER_TIME_LIMIT_MS. Is called automatically when
		 * an object instance is installed as an event filter in a widget.
		 *
		 * \param object Unused
		 * \param event Event to be filtered
		 * \return True when event is a QKeyEvent, false otherwise
		 **/
		bool eventFilter(QObject *object, QEvent *event);
	private:
		///Currently contained card number
		QString cardNumber;
		///Used for measuring time since first character
		QElapsedTimer writeTime;
};

#endif
