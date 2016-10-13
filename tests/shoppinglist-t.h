class ShoppingListTest : QObject {
	Q_OBJECT
	private slots:
		void testNewItem();
		void testSetItemAmount();
		void testDeleteItem();
		void testWipeList();
		void testDeleteLastItem();
		void testGetTotalAmount();
};
