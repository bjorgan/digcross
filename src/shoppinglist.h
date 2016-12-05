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
		 * \param index Index in table. row is assumed to correspond to position in the shopping list. Which item property (name, price, amount) will appear in which column is defined by ITEM_NAME_COL, ITEM_PRICE_COL and ITEM_AMOUNT_COL constants
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

/**
 * Shopping list widget. Uses a QTableView to display the supplied
 * shopping list data model. Uses ShoppingListItemDelegate to draw
 * each shopping list item.
 *
 * This is the class that is used for /drawing/ the shopping list,
 * while all shopping list /data handling/ is done in ShoppingList.
 **/
class ShoppingListWidget : public QWidget {
	Q_OBJECT
	public:
		/**
		 * Creates the table for displaying the shopping list.
		 *
		 * \param list Shopping list
		 **/
		ShoppingListWidget(ShoppingList *list, QWidget *parent = NULL);
	private:
		///Convenience pointer to the canonical shopping list
		ShoppingList *shoppingList;
	private slots:
		/**
		 * Delete shopping list item corresponding to the specified index, given that index.column() corresponds to ITEM_DELETEBUTTON_ROW (user clicked on the cell corresponding to the delete button).
		 *
		 * \param index Index
		 **/
		void deleteShoppingListRow(const QModelIndex &index);
};

/**
 * Used for drawing shopping list rows in the shopping list widget.
 **/
class ShoppingListItemDelegate : public QStyledItemDelegate {
	Q_OBJECT
	public:
		ShoppingListItemDelegate(QObject *parent = NULL);

		/**
		 * Reimplemented from QStyledItemDelegate. Special functionality:
		 * - Draws amount as "NN x"
		 * - Draws price as "(NN kr)"
		 * - In ITEM_DELETEBUTTON_COL column, a delete button is _drawn_.
		 * It is assumed that actual button clicking in the cell is handled elsewhere (i.e. in ShoppingListWidget).
		 **/
		virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

		/**
		 * Reimplemented from QStyledItemDelegate. Create Calculator
		 * widget and return as editor.
		 **/
		virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

		/**
		 * Reimplemented from QStyledItemDelegate. Obtain current
		 * displayed amount from Calculator and set to the model.
		 **/
		virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

		/**
		 * Move calculator widget so that its display overlaps with the
		 * cell that is being edited.
		 **/
		virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
