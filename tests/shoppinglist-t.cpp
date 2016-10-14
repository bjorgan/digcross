#include <QSignalSpy>
#include "shoppinglist-t.h"
#include "shoppinglist.h"
#include <QTableView>

void ShoppingListTest::init()
{
	shoppingList = new ShoppingList;

	//TODO: Since all tests mostly use the same data, populate the shoppingList here.
}

void ShoppingListTest::cleanup()
{
	delete shoppingList;
}

void ShoppingListTest::testNewItem()
{
	QString item_1 = "test_item";
	QString item_2 = "test_item_2";

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
	QCOMPARE(shoppingList->data(shoppingList->index(1, 0)).toString(), item_1);

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
	int amount = 20;
	shoppingList->newItem("test_item_1", 20.0, amount);
	shoppingList->newItem("test_item_2", 20.0, amount);
	shoppingList->newItem("test_item_3", 20.0, amount);

	QCOMPARE(shoppingList->data(shoppingList->index(2, 2)).toInt(), amount);

	//check that the amount is changed when requested
	shoppingList->setItemAmount("test_item_3", amount*2);
	QCOMPARE(shoppingList->data(shoppingList->index(2, 2)).toInt(), amount*2);
}

void ShoppingListTest::testDeleteItem()
{
	int amount = 20;
	shoppingList->newItem("test_item_1", 20.0, amount);
	shoppingList->newItem("test_item_2", 20.0, amount);
	shoppingList->newItem("test_item_3", 20.0, amount);

	QCOMPARE(shoppingList->rowCount(), 3);

	//check that correct row is deleted when using the item name
	shoppingList->deleteItem("test_item_2");
	QCOMPARE(shoppingList->rowCount(), 2);
	QCOMPARE(shoppingList->data(shoppingList->index(0, 0)).toString(), QString("test_item_1"));
	QCOMPARE(shoppingList->data(shoppingList->index(1, 0)).toString(), QString("test_item_3"));

	//check that the correct row is deleted when using the row number
	shoppingList->deleteItem(shoppingList->index(0, 0));
	QCOMPARE(shoppingList->data(shoppingList->index(0, 0)).toString(), QString("test_item_3"));
}

void ShoppingListTest::testWipeList()
{
	int amount = 20;
	shoppingList->newItem("test_item_1", 20.0, amount);
	shoppingList->newItem("test_item_2", 20.0, amount);
	shoppingList->newItem("test_item_3", 20.0, amount);
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
	int amount = 20;
	shoppingList->newItem("test_item_1", 20.0, amount);
	shoppingList->newItem("test_item_2", 20.0, amount);
	shoppingList->newItem("test_item_3", 20.0, amount);
	QCOMPARE(shoppingList->rowCount(), 3);

	//verify that last item was deleted
	shoppingList->deleteLastAddedItem();
	QCOMPARE(shoppingList->rowCount(), 2);
	QCOMPARE(shoppingList->data(shoppingList->index(0, 0)).toString(), QString("test_item_1"));
	QCOMPARE(shoppingList->data(shoppingList->index(1, 0)).toString(), QString("test_item_2"));
}

void ShoppingListTest::testGetTotalAmount()
{
	QCOMPARE(shoppingList->getTotalAmount(), 0.0);
	int amount = 20;
	double price = 20;
	shoppingList->newItem("test_item_1", price, amount);
	QCOMPARE(shoppingList->getTotalAmount(), price*amount*1.0);
	shoppingList->newItem("test_item_2", price, amount);
	QCOMPARE(shoppingList->getTotalAmount(), 2*price*amount*1.0);
	shoppingList->newItem("test_item_3", price, amount);
	QCOMPARE(shoppingList->getTotalAmount(), 3*price*amount*1.0);
}

QTEST_MAIN(ShoppingListTest)
