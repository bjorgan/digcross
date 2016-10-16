#include "shoppinglist.h"

ShoppingList::ShoppingList(QObject *parent) : QAbstractTableModel(parent)
{
}

void ShoppingList::newItem(QString itemName, double price, int amount)
{
	if (!items.contains(itemName)) { //item is new, will insert new row
		itemRows.push_back(itemName);
		int row = numItems();

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

	emit totalPriceChanged();
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
		beginRemoveRows(QModelIndex(), 0, numItems()-1);

		items.clear();
		itemRows.clear();

		endRemoveRows();
	}
}

void ShoppingList::deleteLastAddedItem()
{
	deleteItem(getItemName(numItems()-1));
}

double ShoppingList::getTotalPrice()
{
	double totalAmount = 0;
	for (int i=0; i < numItems(); i++) {
		ShoppingListItem item = items[getItemName(i)];
		totalAmount += item.amount*item.price;
	}
	return totalAmount;
}

QString ShoppingList::getItemName(int row) const
{
	if ((row >= 0) && (row < numItems()) && (numItems() > 0)) {
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

int ShoppingList::numItems() const
{
	return items.size();
}

/**
 * Neccessary functions for QAbstractTableModel subclassing.
 **/

int ShoppingList::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid()) {
		return numItems();
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
				return item.amount*item.price;
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

QVariant ShoppingList::headerData(int section, Qt::Orientation orientation, int role) const
{
	if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole)) {
		switch (section) {
			case ITEM_NAME_COL:
				return QVariant(tr("Name"));
			case ITEM_PRICE_COL:
				return QVariant(tr("Price"));
			case ITEM_AMOUNT_COL:
				return QVariant(tr("Amount"));
		}
	}

	return QAbstractTableModel::headerData(section, orientation, role);
}

/**
 * QAbstractTableModel edit functions.
 **/

bool ShoppingList::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.column() == ITEM_AMOUNT_COL) {
		setItemAmount(getItemName(index.row()), value.toInt());
		emit dataChanged(index, index);
		emit totalPriceChanged();
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

void ShoppingListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &inputOption, const QModelIndex &index) const Q_DECL_OVERRIDE
{
	QStyleOptionViewItem option = inputOption;
	if (index.column() == ITEM_AMOUNT_COL) {
		option.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
		painter->drawText(option.rect, option.displayAlignment, index.data().toString() + " x ");
	} else if (index.column() == ITEM_PRICE_COL) {
		option.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
		painter->drawText(option.rect, option.displayAlignment, "(" + index.data().toString() + " kr)");
	} else {
		QStyledItemDelegate::paint(painter, inputOption, index);
	}
}

ShoppingListWidget::ShoppingListWidget(ShoppingList *list, QWidget *parent) : QWidget(parent), shoppingList(list)
{
	//view for displaying shopping list
	QTableView *listView = new QTableView;
	listView->verticalHeader()->hide();
	listView->setAlternatingRowColors(true);
	listView->setGridStyle(Qt::NoPen);
	listView->setModel(list);
	listView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	listView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	listView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

	//use custom delegate for displaying each item
	listView->setItemDelegate(new ShoppingListItemDelegate);

	//current sum
	currentTotalPrice = new QLabel;

	//layout
	QGridLayout *layout = new QGridLayout(this);
	int row = 0;
	int col = 0;

	layout->addWidget(listView, row, col);
	layout->addWidget(currentTotalPrice, ++row, col, Qt::AlignLeft);

	updateDisplayPrice();

	//connections
	connect(list, SIGNAL(totalPriceChanged()), SLOT(updateDisplayPrice()));
}

void ShoppingListWidget::updateDisplayPrice()
{
	double currPrice = shoppingList->getTotalPrice();
	currentTotalPrice->setText("Sum: " + QString::number(currPrice) + "kr");
}
