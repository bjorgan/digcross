#include <QObject>
#include <QAbstractItemModel>
#include <QMap>

///Number of properties associated with each shopping list item (price, name, amount)
const int NUM_SHOPPINGLIST_PROPERTIES = 3;
///Column index for name in shopping list model
#define ITEM_NAME_COL 0
///Column index for price in shopping list model
#define ITEM_PRICE_COL 1
///Column index for amount in shopping list model
#define ITEM_AMOUNT_COL 2


typedef struct {
	int amount;
	double price;
} ShoppingListItem;

typedef QMap<QString, ShoppingListItem> ShoppingListData;

/**
 * Data structure for containing a shoppinglist. Inherits from QAbstractTableModel, and is
 * thus ready for display in a QAbstractItemView-derived widget.
 **/
class ShoppingList : public QAbstractTableModel {
	Q_OBJECT
	public:
		ShoppingList(QObject *parent = NULL);

		/**
		 * Get total amount in shopping list.
		 **/
		double getTotalAmount();

		/**
		 * Neccessary functions for QAbstractTableModel subclassing.
		 **/
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
		Qt::ItemFlags flags(const QModelIndex &index) const;
	public slots:
		/**
		 * Add new item to shopping list. (Will replace any existing items
		 * with same name.)
		 *
		 * \param itemName Name of item (unique)
		 * \param price Item price
		 * \param amount Item amount
		 **/
		void newItem(QString itemName, double price, int amount = 1);

		/**
		 * Set new amount for item.
		 *
		 * \param itemName Item name
		 * \param amount Amount
		 **/
		void setItemAmount(QString itemName, int amount);

		/**
		 * Convenience function for setting amount from a QAbstractItemView-derived class.
		 *
		 * \param index Model index
		 * \param amount Amount
		 **/
		void setItemAmount(const QModelIndex &index, int amount);

		/**
		 * Delete item from shopping list.
		 *
		 * \param itemName Item
		 **/
		void deleteItem(QString itemName);

		/**
		 * Convenience function for deleting item from list from a QAbstractItemView-derived class.
		 **/
		void deleteItem(const QModelIndex &index);

		/**
		 * Wipe entire shopping list.
		 **/
		void wipeList();

		/**
		 * Delete last added item.
		 **/
		void deleteLastAddedItem();
	private:
		///List over shopping list entries
		ShoppingListData items;
		///Shopping list row number associated with each entry in the
		///shopping list, in order to have a well-defined ordering of
		///the elements
		QVector<QString> itemRows;

		/**
		 * Set price of item.
		 *
		 * \param itemName Item
		 * \param price New price
		 **/
		void setItemPrice(QString itemName, double price);

		/**
		 * Get item name associated with a given position.
		 *
		 * \param row Row number
		 **/
		QString getItemName(int row) const;
};
