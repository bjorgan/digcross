#include <QObject>
#include <QtTest/QTest>

class ShoppingList;

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
		void testGetTotalPrice();
	private:
		ShoppingList *shoppingList;
};
