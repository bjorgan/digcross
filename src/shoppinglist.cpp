#include "shoppinglist.h"
#include <iostream>
#include <QHash>

ShoppingList::ShoppingList(QObject *parent) : QAbstractTableModel(parent)
{
}

void ShoppingList::newItem(QString itemName, double price, int amount)
{
	int row = 0;
	bool newRow = false;
	if (!items.contains(itemName)) {
		itemRows.push_back(itemName);
	}

	//TODO: call correct rowChanged, rowInserted functions. 
	setItemPrice(itemName, price);
	setItemAmount(itemName, amount);
}

void ShoppingList::setItemPrice(QString itemName, double price)
{
	if (price > 0) {
		items[itemName].price = price;
	}
}

void ShoppingList::setItemAmount(QString itemName, int amount)
{
	if (amount > 0) {
		items[itemName].amount = amount;
	}
}

void ShoppingList::deleteItem(QString itemName)
{
	items.erase(items.find(itemName));
	itemRows.erase(itemRows.begin() + itemRows.lastIndexOf(itemName));
}

void ShoppingList::wipeList()
{
	items.clear();
	itemRows.clear();
}

void ShoppingList::deleteLastAddedItem()
{
	deleteItem(getItemName(rowCount()-1));
}

double ShoppingList::getTotalAmount()
{
	double totalAmount = 0;
	for (int i=0; i < rowCount(); i++) {
		ShoppingListItem item = items[getItemName(i)];
		totalAmount += item.amount*item.price;
	}
	return totalAmount;
}

/**
 * QAbstractTableModel subclassing convenience functions.
 **/

void ShoppingList::setItemAmount(const QModelIndex &parent, int amount)
{
	setItemAmount(getItemName(parent.row()), amount);
}

void ShoppingList::deleteItem(const QModelIndex &parent)
{
	deleteItem(getItemName(parent.row()));
}

/**
 * Neccessary functions for QAbstractTableModel subclassing.
 **/

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
	if ((role == Qt::DisplayRole)) {
		QString itemName = getItemName(index.row());
		if (itemName == QString()) {
			return QVariant(QVariant::Invalid);
		}

		ShoppingListItem item = items[itemName];
		int column = index.column();

		switch (column) {
			case ITEM_NAME_COL:
				return itemName;
			case ITEM_PRICE_COL:
				return item.price;
			case ITEM_AMOUNT_COL:
				return item.amount;
		}
	}

	return QVariant(QVariant::Invalid);
}

Qt::ItemFlags ShoppingList::flags(const QModelIndex &index) const
{
	if (index.column() == ITEM_AMOUNT_COL) {
		return Qt::ItemIsEnabled | Qt::ItemIsEditable;
	} else {
		return Qt::NoItemFlags;
	}
}

/**
 * QAbstractTableModel edit functions.
 **/

bool ShoppingList::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.column() == ITEM_AMOUNT_COL) {
		setItemAmount(getItemName(index.row()), value.toInt());
		emit dataChanged(index, index);
		return true;
	}

	return false;
}

QString ShoppingList::getItemName(int row) const
{
	if (row < rowCount() && (rowCount() > 0)) {
		return itemRows[row];
	} else {
		return QString();
	}
}
