#include <QWidget>

class ShoppingList;

/**
 * Mainwindow container widget opened when starting digcross.
 **/
class MainWindow : public QWidget {
	Q_OBJECT
	public:
		MainWindow(QWidget *parent = NULL);
	private:
		ShoppingList *shoppingList;
};
