#include <QObject>
#include <QAbstractItemModel>
#include <QMap>

///Number of properties associated with each shopping list item (price, name, amount)
const int NUM_SHOPPINGLIST_PROPERTIES = 3;

#define ITEM_NAME_COL 0
#define ITEM_PRICE_COL 1
#define ITEM_AMOUNT_COL 2

typedef QMap<QString, std::pair<int, double> > ShoppingListDB;

class ShoppingList : public QAbstractTableModel {
	Q_OBJECT
	public:
		ShoppingList(QObject *parent = NULL);

		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		double getTotalAmount();
	public slots:
		void newItem(QString itemName, double price, int amount = 1);
		void setItemAmount(QString itemName, int amount);
		void setItemAmount(const QModelIndex &parent, int amount);

		void deleteItem(QString itemName);
		void deleteItem(const QModelIndex &parent);

		void wipeList();
//		void deleteLastAddedItem();
	private:
		///List over prices associated with each item. Contained here
		//since price menu can potentially have arbitrary prices for
		//each unit, and the price list will not contain these.
		ShoppingListDB items;
		void setItemPrice(QString itemName, double price);
		ShoppingListDB::const_iterator getItem(const QModelIndex &parent) const;
		ShoppingListDB::iterator getItem(const QModelIndex &parent);
};
