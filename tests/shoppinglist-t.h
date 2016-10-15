#include <QObject>
#include <QtTest/QTest>

class ShoppingList;
class QTableView;

class ShoppingListTest : public QObject {
	Q_OBJECT
	private slots:
		void init();
		void cleanup();

		void testNewItem();
		void testSetItemAmount();
		void testDeleteItem();
		void testWipeList();
		void testDeleteLastItem();
		void testGetTotalAmount();
	private:
		ShoppingList *shoppingList;
};