#include <QObject>
#include <QtTest/QTest>

class ShoppingList;

class ShoppingListTest : public QObject {
	Q_OBJECT
	private slots:
		/**
		 * Create/clean up shopping list, populate shopping list with some default items.
		 **/
		void init();
		void cleanup();

		/**
		 * Check that new item gets added, that new item with same name overwrites current item.
		 **/
		void testNewItem();

		/**
		 * Check that amount gets set.
		 **/
		void testSetItemAmount();

		/**
		 * Check that item is deleted.
		 **/
		void testDeleteItem();

		/**
		 * Check that items are wiped.
		 **/
		void testWipeList();

		/**
		 * Check that last item was deleted.
		 **/
		void testDeleteLastItem();

		/**
		 * Check that calculated total price is correct.
		 **/
		void testGetTotalPrice();
	private:
		///Shopping list
		ShoppingList *shoppingList;
};
