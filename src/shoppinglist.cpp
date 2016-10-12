#include "shoppinglist.h"

ShoppingList::ShoppingList(QObject *parent) : QAbstractTableModel(parent)
{
}

void ShoppingList::newItem(QString itemName, double price, int amount)
{
	setItemPrice(itemName, price);
	setItemAmount(itemName, amount);
}

void ShoppingList::setItemPrice(QString itemName, double price)
{
	items[itemName].second = price;
}

void ShoppingList::setItemAmount(QString itemName, int amount)
{
	items[itemName].first = amount;
}

int ShoppingList::rowCount(const QModelIndex &parent) const
{
	return items.size();
}

int ShoppingList::columnCount(const QModelIndex &parent) const
{
	return NUM_SHOPPINGLIST_PROPERTIES;
}

QVariant ShoppingList::data(const QModelIndex &index, int role) const
{
	ShoppingListDB::const_iterator item = getItem(index);
	int column = index.column();

	switch (column) {
		case ITEM_NAME_COL:
			return QVariant(item.key());
		case ITEM_PRICE_COL:
			return QVariant(item->second);
		case ITEM_AMOUNT_COL:
			return QVariant(item->first);
	}
}

void ShoppingList::setItemAmount(const QModelIndex &parent, int amount)
{
	setItemAmount(getItem(parent).key(), amount);
}

void ShoppingList::deleteItem(QString itemName)
{
	items.erase(items.find(itemName));
}

void ShoppingList::deleteItem(const QModelIndex &parent)
{
	deleteItem(getItem(parent).key());
}

void ShoppingList::wipeList()
{
	items.clear();
}

ShoppingListDB::const_iterator ShoppingList::getItem(const QModelIndex &parent) const
{
	return (items.begin() + parent.row());
}

ShoppingListDB::iterator ShoppingList::getItem(const QModelIndex &parent)
{
	return (items.begin() + parent.row());
}
