#include "shoppinglist.h"

///Number of properties associated with each shopping list item (price, name, amount)
const int NUM_SHOPPINGLIST_PROPERTIES = 3;
///Column index for name in shopping list model
#define ITEM_NAME_COL 1
///Column index for price in shopping list model
#define ITEM_PRICE_COL 2
///Column index for amount in shopping list model
#define ITEM_AMOUNT_COL 0

ShoppingList::ShoppingList(QObject *parent) : QAbstractTableModel(parent)
{
}

void ShoppingList::newItem(QString itemName, double price, int amount)
{
	if (!items.contains(itemName)) { //item is new, will insert new row
		itemRows.push_back(itemName);
		int row = rowCount();

		beginInsertRows(QModelIndex(), row, row);
		setItemPrice(itemName, price);
		setItemAmount(itemName, amount);
		endInsertRows();
	} else if (items[itemName].price == price) { //item exists, will add to the existing amount
		int row = itemRows.lastIndexOf(itemName);

		setItemAmount(itemName, items[itemName].amount + amount);
		emit dataChanged(index(row, 0), index(row, NUM_SHOPPINGLIST_PROPERTIES));
	} else { //ignore new item, price was not the same
		return;
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

#include <QGridLayout>
#include <QTableView>
#include <QLabel>
#include <QHeaderView>
#include <QPainter>

ShoppingListItemDelegate::ShoppingListItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

void ShoppingListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE
{
	if (index.column() == ITEM_AMOUNT_COL) {
		painter->drawText(option.rect, option.displayAlignment, index.data().toString());
	} else {
		QStyledItemDelegate::paint(painter, option, index);
	}
}

ShoppingListWidget::ShoppingListWidget(ShoppingList *list, QWidget *parent) : QWidget(parent)
{
	//view for displaying shopping list
	QTableView *listView = new QTableView;
	listView->horizontalHeader()->hide();
	listView->verticalHeader()->hide();
	listView->setGridStyle(Qt::NoPen);
	listView->setModel(list);

	//use custom delegate for displaying each item
	listView->setItemDelegate(new ShoppingListItemDelegate);

	//layout
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(new QLabel(tr("Sum: ")), 0, 0);
	QLabel *currSum = new QLabel("0");
	layout->addWidget(currSum, 0, 1);
	layout->addWidget(listView);
}
