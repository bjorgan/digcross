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

typedef QMap<QString, std::pair<int, double> > ShoppingListData;

class ShoppingList : public QAbstractTableModel {
	Q_OBJECT
	public:
		ShoppingList(QObject *parent = NULL);

		/**
		 * Get total amount in shopping list.
		 **/
		double getTotalAmount();

		/**
		 * Neccessary functions for QAbstractTableModel subclassing (read-only).
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
		 * Convenience function for setting amount from a view.
		 *
		 * \param parent Model index
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
		 * Convenience function for deleting item from list from a view.
		 **/
		void deleteItem(const QModelIndex &parent);

		/**
		 * Wipe entire shopping list.
		 **/
		void wipeList();
//		void deleteLastAddedItem();
	private:
		///Shopping list data structure
		ShoppingListData items;

		/**
		 * Set price of item.
		 *
		 * \param itemName Item
		 * \param price New price
		 **/
		void setItemPrice(QString itemName, double price);

		/**
		 * Get iterator to item in shopping list. Used for model/view convenience.
		 *
		 * \param index Index
		 **/
		ShoppingListData::const_iterator getItem(const QModelIndex &index) const;
		ShoppingListData::iterator getItem(const QModelIndex &index);
};
