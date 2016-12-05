#include "shoppinglist.h"
#include "calculator.h"

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
		emit dataChanged(index(row, 0), index(row, columnCount()));
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

		emit totalPriceChanged();
	}
}

void ShoppingList::wipeList()
{
	if (items.size() > 0) {
		beginRemoveRows(QModelIndex(), 0, numItems()-1);

		items.clear();
		itemRows.clear();

		endRemoveRows();

		emit totalPriceChanged();
	}
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
	return NUM_SHOPPINGLIST_PROPERTIES + 1; //an extra column for displaying a delete button
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
	Qt::ItemFlags retFlags = Qt::ItemIsEnabled;
	if (index.column() == ITEM_AMOUNT_COL) {
		retFlags |= Qt::ItemIsEditable;
	}
	return retFlags;
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
#include <QPushButton>
#include <QApplication>
#include <QStyleOptionButton>
#include <QPixmap>
#include <QBitmap>
#include <QDesktopWidget>

ShoppingListItemDelegate::ShoppingListItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

void ShoppingListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &inputOption, const QModelIndex &index) const
{
	QStyleOptionViewItem option = inputOption;
	option.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
	if (index.column() == ITEM_AMOUNT_COL) {
		//draw amount + "x"
		option.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
		painter->drawText(option.rect, option.displayAlignment, index.data().toString() + " x ");
	} else if (index.column() == ITEM_PRICE_COL) {
		//draw price + "kr"
		painter->drawText(option.rect, option.displayAlignment, "(" + index.data().toString() + " kr)");
	} else if (index.column() == ITEM_NAME_COL) {
		painter->drawText(option.rect, option.displayAlignment, index.data().toString());
	} else if (index.column() == ITEM_DELETEBUTTON_COL) {
		//draw delete button in its assigned column
		QStyleOptionButton buttonOptions;
		buttonOptions.features = QStyleOptionButton::Flat;
		buttonOptions.rect = inputOption.rect;
		buttonOptions.state = inputOption.state;
		buttonOptions.icon = qApp->style()->standardIcon(QStyle::SP_DialogCancelButton);
		buttonOptions.iconSize = inputOption.rect.size();
		qApp->style()->drawControl(QStyle::CE_PushButton, &buttonOptions, painter);
	} else {
		QStyledItemDelegate::paint(painter, inputOption, index);
	}
}

QWidget* ShoppingListItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Calculator *calculator = new Calculator(option.rect.height(), option.rect.width(), parent);

	//make calculator stay on top of everything else
	calculator->setWindowFlags(Qt::Popup);

	calculator->layout()->activate(); //force layout update

	return calculator;
}

void ShoppingListItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QWidget *parent = qobject_cast<QWidget*>(editor->parent());
	QPoint parentAbsolutePos = parent->mapToGlobal(parent->pos());
	Calculator *calculator = qobject_cast<Calculator*>(editor);

	//flip display field position if the space left on the screen is too small
	int remainingScreenHeight = QApplication::desktop()->screenGeometry().height() - parentAbsolutePos.y();
	int calculatorEndPosition = option.rect.y() + calculator->height();
	if (remainingScreenHeight < calculatorEndPosition) {
		calculator->setDisplayPos(Calculator::DISPLAY_ON_BOTTOM);
	} else {
		calculator->setDisplayPos(Calculator::DISPLAY_ON_TOP);
	}

	//move calculator so that its display field is positioned directly above the amount field
	QPoint calculatorDisplayPos = calculator->displayPos();
	editor->move(parentAbsolutePos.x() + option.rect.x(), parentAbsolutePos.y() + option.rect.y() - calculatorDisplayPos.y());

	//make the calculator background become transparent
	calculator->setAutoFillBackground(false);
	QPixmap pixmap = calculator->grab();
	calculator->setMask(pixmap.createHeuristicMask());
}

void ShoppingListItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	Calculator *calculator = qobject_cast<Calculator*>(editor);
	model->setData(index, QVariant(calculator->getDisplayedAmount()));
}

ShoppingListWidget::ShoppingListWidget(ShoppingList *list, QWidget *parent) : QWidget(parent), shoppingList(list)
{
	//view for displaying shopping list
	QTableView *listView = new QTableView;
	listView->verticalHeader()->hide();
	listView->horizontalHeader()->hide();
	listView->setAlternatingRowColors(true);
	listView->setShowGrid(false);
	listView->setEditTriggers(QAbstractItemView::DoubleClicked);
	listView->setModel(list);
	listView->horizontalHeader()->setSectionResizeMode(ITEM_PRICE_COL, QHeaderView::Stretch);
	listView->setEditTriggers(QAbstractItemView::AllEditTriggers);
	listView->resizeColumnToContents(ITEM_DELETEBUTTON_COL);

	//use custom delegate for displaying each item
	listView->setItemDelegate(new ShoppingListItemDelegate);

	//layout
	QGridLayout *layout = new QGridLayout(this);
	int row = 0;
	int col = 0;

	layout->addWidget(listView, row, col, 1, 2);

	//connections
	connect(listView, SIGNAL(clicked(const QModelIndex &)), SLOT(deleteShoppingListRow(const QModelIndex &)));
}

void ShoppingListWidget::deleteShoppingListRow(const QModelIndex &index)
{
	if (index.isValid() && (index.column() == ITEM_DELETEBUTTON_COL)) {
		//signal came from clicking on the delete button-cell, so can safely delete the row
		shoppingList->deleteItem(index);
	}
}
