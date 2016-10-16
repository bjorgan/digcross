#include <QObject>
#include <QAbstractTableModel>
#include <QMap>

///Number of properties associated with each shopping list item (price, name, amount)
const int NUM_SHOPPINGLIST_PROPERTIES = 3;
///Column index for row delete button
#define ITEM_DELETEBUTTON_COL 3
///Column index for name in shopping list model
#define ITEM_NAME_COL 1
///Column index for price in shopping list model
#define ITEM_PRICE_COL 2
///Column index for amount in shopping list model
#define ITEM_AMOUNT_COL 0

/**
 * Item in shopping list.
 **/
typedef struct {
	///Amount of item
	int amount;
	///Price of item
	double price;
} ShoppingListItem;

/**
 * Data structure for containing a shoppinglist. Inherits from QAbstractTableModel, and is
 * thus ready for display in a QAbstractItemView-derived widget.
 *
 * New items should be added by connecting to the ShoppingList::newItem()-SLOT.
 *
 * ShoppingList will notify any connected View with changes.
 **/
class ShoppingList : public QAbstractTableModel {
	Q_OBJECT
	public:
		ShoppingList(QObject *parent = NULL);

		/**
		 * Get total price of all items in shopping list.
		 **/
		double getTotalPrice();

		/**
		 * Get number of items in shopping list.
		 **/
		int numItems() const;

		/**
		 * \defgroup TableModelFunctions QAbstractTableModel subclassing
		 * Functions related to the QAbstractTableModel subclassing.
		 **/

		/**
		 * \ingroup TableModelFunctions
		 * Number of rows in the table model represented by ShoppingList. Necessary for QAbstractTableModel subclassing.
		 *
		 * \param parent Parent index
		 * \return Number of items in shopping list
		 **/
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

		/**
		 * \ingroup TableModelFunctions
		 * Number of columns in the table model. Necessary for QAbstractTableModel subclassing.
		 *
		 * \param parent Parent index
		 * \return Always 3 (name + price + amount)
		 **/
		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

		/**
		 * \ingroup TableModelFunctions
		 * Data access function. Necessary for QAbstractTableModel subclassing. Implements only Qt::DisplayRole.
		 *
		 * \param index Index in table. row is assumed to correspond to position in the shopping list, while columns 0, 1 and 2 correspond to name, price and amount (see also ITEM_NAME_COL etc. defined in shoppinglist.cpp)
		 * \param role Role, see Qt docs for QAbstractItemModel
		 * \return Name, price, amount or QVariant(QVariant::Invalid) depending on column index
		 **/
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

		/**
		 * \ingroup TableModelFunctions
		 * Data setting function. Necessary for QAbstractTableModel subclassing. Implements only Qt::EditRole, allows editing only for the amount of an already inserted item.
		 *
		 * \param index Index in table, see documentation for data()
		 * \param value New value for the specified table position
		 * \param role Role
		 **/
		virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

		/**
		 * \ingroup TableModelFunctions
		 * Item flags for the defined table indices.
		 *
		 * \param index Index, see documentation for data()
		 * \return Enabled and editable if the index corresponds to an amount for a shopping list item, disabled otherwise
		 **/
		virtual Qt::ItemFlags flags(const QModelIndex &index) const;

		/**
		 * \ingroup TableModelFunctions
		 * Reimplemented from QAbstractItemModel. Returns custom headers for the horizontal orientation.
		 **/
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	public slots:
		/**
		 * Add new item to shopping list. Item names function as unique identifiers:
		 * 	- If the price is the same: Assumed that the item is the same, the input amount is
		 * 	  added to the existing item amount
		 * 	- If the price is different: This should happen, so the new item is ignored. :-P
		 *
		 * \param itemName Name of item (unique)
		 * \param price Item price
		 * \param amount Item amount (number of items, is accumulated to the item each time the SLOT is called)
		 **/
		void newItem(QString itemName, double price, int amount = 1);

		/**
		 * Set new amount of item.
		 *
		 * \param itemName Item name
		 * \param amount Amount
		 **/
		void setItemAmount(QString itemName, int amount);

		/**
		 * Delete item from shopping list.
		 *
		 * \param itemName Item
		 **/
		void deleteItem(QString itemName);

		/**
		 * Wipe entire shopping list.
		 **/
		void wipeList();

		/**
		 * Delete last added (new) item.
		 **/
		void deleteLastAddedItem();

		/**
		 * \ingroup TableModelFunctions
		 * Convenience function for setting item amount from a QAbstractItemView-derived subclass.
		 *
		 * \param index Item index, see documentation for data()
		 * \param amount New amount of item at the specified row-number
		 **/
		void setItemAmount(const QModelIndex &index, int amount);

		/**
		 * \ingroup TableModelFunctions
		 * Convenience function for deleting an item from a QAbstractItemView-derived subclass.
		 *
		 * \param index Item index, see documentation for data()
		 **/
		void deleteItem(const QModelIndex &index);
	private:
		///List over shopping list entries, ordered by item name
		QMap<QString, ShoppingListItem> items;
		///Shopping list row numbers, for associating a well-defined row ordering with the shopping list
		QVector<QString> itemRows;

		/**
		 * Set price of item.
		 *
		 * \param itemName Item
		 * \param price New price
		 **/
		void setItemPrice(QString itemName, double price);

		/**
		 * Get item name associated with a given row along itemRows.
		 *
		 * \param row Row number
		 **/
		QString getItemName(int row) const;
	signals:
		/**
		 * Emitted whenever the total price changes. Emitted from newItem() and setData().
		 **/
		void totalPriceChanged();
};

#include <QWidget>
#include <QStyledItemDelegate>

class QLabel;

class ShoppingListItemDelegate : public QStyledItemDelegate {
	Q_OBJECT
	public:
		ShoppingListItemDelegate(QObject *parent = NULL);
		virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};


class ShoppingListWidget : public QWidget {
	Q_OBJECT
	public:
		ShoppingListWidget(ShoppingList *list, QWidget *parent = NULL);
	private:
		ShoppingList *shoppingList;
		QLabel *currentTotalPrice;
	private slots:
		void updateDisplayPrice();
};
