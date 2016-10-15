#include "shoppinglist.h"
#include <iostream>
#include <QHash>

///Number of properties associated with each shopping list item (price, name, amount)
const int NUM_SHOPPINGLIST_PROPERTIES = 3;
///Column index for name in shopping list model
#define ITEM_NAME_COL 0
///Column index for price in shopping list model
#define ITEM_PRICE_COL 1
///Column index for amount in shopping list model
#define ITEM_AMOUNT_COL 2

ShoppingList::ShoppingList(QObject *parent) : QAbstractTableModel(parent)
{
}

void ShoppingList::newItem(QString itemName, double price, int amount)
{
	//find whether new row should be added, or whether we should change existing item
	int row = 0;
	bool isNewRow = false;
	if (!items.contains(itemName)) {
		itemRows.push_back(itemName);
		row = rowCount();
		isNewRow = true;
		beginInsertRows(QModelIndex(), row, row); //alert connected views
	} else {
		row = itemRows.lastIndexOf(itemName);
		isNewRow = false;
	}

	//add/change item
	setItemPrice(itemName, price);
	setItemAmount(itemName, amount);

	//alert connected views
	if (isNewRow) {
		endInsertRows(); //alert connected views
	} else {
		emit dataChanged(index(row, 0), index(row, NUM_SHOPPINGLIST_PROPERTIES));
	}
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
	if (items.contains(itemName)) {
		int row = itemRows.lastIndexOf(itemName);
		beginRemoveRows(QModelIndex(), row, row);

		items.erase(items.find(itemName));
		itemRows.erase(itemRows.begin() + row);

		endRemoveRows();
	}
}

void ShoppingList::wipeList()
{
	if (items.size() > 0) {
		beginRemoveRows(QModelIndex(), 0, rowCount()-1);

		items.clear();
		itemRows.clear();

		endRemoveRows();
	}
}

void ShoppingList::deleteLastAddedItem()
{
	deleteItem(getItemName(rowCount()-1));
}

double ShoppingList::getTotalPrice()
{
	double totalAmount = 0;
	for (int i=0; i < rowCount(); i++) {
		ShoppingListItem item = items[getItemName(i)];
		totalAmount += item.amount*item.price;
	}
	return totalAmount;
}

QString ShoppingList::getItemName(int row) const
{
	if ((row >= 0) && (row < rowCount()) && (rowCount() > 0)) {
		return itemRows[row];
	} else {
		return QString();
	}
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
	if (!parent.isValid()) {
		return items.size();
	}

	return 0;
}

int ShoppingList::columnCount(const QModelIndex &parent) const
{
	return NUM_SHOPPINGLIST_PROPERTIES;
}

QVariant ShoppingList::data(const QModelIndex &index, int role) const
{
	QString itemName = getItemName(index.row());
	if ((role == Qt::DisplayRole) && (itemName != QString())) {
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
