#include <QObject>
#include <QAbstractItemModel>
#include <QMap>

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
		 * Neccessary functions for QAbstractTableModel subclassing.
		 **/
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
		virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	public slots:
		/**
		 * Add new item to shopping list. (Will replace any existing items
		 * with same name.)
		 *
		 * \param itemName Name of item (unique)
		 * \param price Item price
		 * \param amount Item amount (number of items)
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
		 * Delete last added item.
		 **/
		void deleteLastAddedItem();

		/**
		 * Convenience functions for QAbstractItemView-derived subclasses.
		 **/
		void setItemAmount(const QModelIndex &index, int amount);
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
		 * Get item name associated with a given position.
		 *
		 * \param row Row number
		 **/
		QString getItemName(int row) const;
};
