#include <QSignalSpy>
#include "shoppinglist-t.h"
#include "shoppinglist.h"
#include <QTableView>

const QString ITEM_1 = "test_item_1";
const QString ITEM_2 = "test_item_2";
const QString ITEM_3 = "test_item_3";
const double PRICE = 20.0;
const int AMOUNT = 20;

void ShoppingListTest::init()
{
	shoppingList = new ShoppingList;

	//add test data
	shoppingList->newItem(ITEM_1, PRICE, AMOUNT);
	shoppingList->newItem(ITEM_2, PRICE, AMOUNT);
	shoppingList->newItem(ITEM_3, PRICE, AMOUNT);
}

void ShoppingListTest::cleanup()
{
	delete shoppingList;
}

void ShoppingListTest::testNewItem()
{
	//start from scratch (ignore test data created in init()
	delete shoppingList;
	shoppingList = new ShoppingList;

	QString item_1 = "test_item_c";
	QString item_2 = "test_item_b";

	double price = 20.0;
	int amount = 10;

	//basic test of add
	QCOMPARE(shoppingList->rowCount(), 0);
	shoppingList->newItem(item_1, price, amount);
	QCOMPARE(shoppingList->data(shoppingList->index(0, 0)).toString(), item_1);

	//add another item, verify it is added in a new row
	QCOMPARE(shoppingList->rowCount(), 1);
	shoppingList->newItem(item_2, price, amount);
	QCOMPARE(shoppingList->rowCount(), 2);
	QCOMPARE(shoppingList->data(shoppingList->index(1, 0)).toString(), item_2);

	//verify the other elements
	QCOMPARE(shoppingList->data(shoppingList->index(1, 1)).toDouble(), price);
	QCOMPARE(shoppingList->data(shoppingList->index(1, 2)).toInt(), amount);

	//add item with same name, verify it overwrites the former item
	double price_2 = 30.0;
	int amount_2 = 30;
	shoppingList->newItem(item_2, price_2, amount_2);
	QCOMPARE(shoppingList->rowCount(), 2);
	QCOMPARE(shoppingList->data(shoppingList->index(1, 1)).toDouble(), price_2);
	QCOMPARE(shoppingList->data(shoppingList->index(1, 2)).toInt(), amount_2);
}

void ShoppingListTest::testSetItemAmount()
{
	QCOMPARE(shoppingList->data(shoppingList->index(2, 2)).toInt(), AMOUNT);

	//check that the amount is changed when requested
	shoppingList->setItemAmount(ITEM_3, AMOUNT*2);
	QCOMPARE(shoppingList->data(shoppingList->index(2, 2)).toInt(), AMOUNT*2);
}

void ShoppingListTest::testDeleteItem()
{
	QCOMPARE(shoppingList->rowCount(), 3);

	//check that correct row is deleted when using the item name
	shoppingList->deleteItem(ITEM_2);
	QCOMPARE(shoppingList->rowCount(), 2);
	QCOMPARE(shoppingList->data(shoppingList->index(0, 0)).toString(), ITEM_1);
	QCOMPARE(shoppingList->data(shoppingList->index(1, 0)).toString(), ITEM_3);

	//check that the correct row is deleted when using the row number
	shoppingList->deleteItem(shoppingList->index(0, 0));
	QCOMPARE(shoppingList->data(shoppingList->index(0, 0)).toString(), ITEM_3);
}

void ShoppingListTest::testWipeList()
{
	QCOMPARE(shoppingList->rowCount(), 3);

	//verify that list is wiped
	shoppingList->wipeList();
	QCOMPARE(shoppingList->rowCount(), 0);
	QCOMPARE(shoppingList->data(shoppingList->index(0, 0)), QVariant(QVariant::Invalid));
	shoppingList->wipeList();
	QCOMPARE(shoppingList->rowCount(), 0);
}

void ShoppingListTest::testDeleteLastItem()
{
	shoppingList->newItem("something_different", 20, 20);
	QCOMPARE(shoppingList->rowCount(), 4);

	//verify that last item was deleted
	shoppingList->deleteLastAddedItem();
	QCOMPARE(shoppingList->rowCount(), 3);
	QCOMPARE(shoppingList->data(shoppingList->index(0, 0)).toString(), ITEM_1);
	QCOMPARE(shoppingList->data(shoppingList->index(1, 0)).toString(), ITEM_2);
	QCOMPARE(shoppingList->data(shoppingList->index(2, 0)).toString(), ITEM_3);
}

void ShoppingListTest::testGetTotalPrice()
{
	shoppingList->wipeList();
	QCOMPARE(shoppingList->rowCount(), 0);

	int amount = 20;
	double price = 20;
	shoppingList->newItem(ITEM_1, price, amount);
	QCOMPARE(shoppingList->getTotalPrice(), price*amount*1.0);
	shoppingList->newItem(ITEM_2, price, amount);
	QCOMPARE(shoppingList->getTotalPrice(), 2*price*amount*1.0);
	shoppingList->newItem(ITEM_3, price, amount);
	QCOMPARE(shoppingList->getTotalPrice(), 3*price*amount*1.0);
}

QTEST_MAIN(ShoppingListTest)
