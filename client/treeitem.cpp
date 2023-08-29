#include "treeitem.h"
#include <QDebug>
TreeItem::TreeItem (const QVector<QVariant> &data, TreeItem *parent) {
    m_parentItem = parent;
    m_itemData = data;
}

TreeItem::~TreeItem() { qDeleteAll(m_childItems); }

void TreeItem::appendChild(TreeItem *item) {
    m_childItems.append(item);
}

TreeItem *TreeItem::child (int row) {
    return m_childItems.value(row);
}

int TreeItem::childCount() const {
    return m_childItems.count();
}

int TreeItem::columnCount() const {
    return m_itemData.count();
}

QVariant TreeItem::data (int column) const {
    return m_itemData.value(column);
}

TreeItem *TreeItem::parentItem() {
    return m_parentItem;
}

int TreeItem::childNumber() const {
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));
    return 0;
}

bool TreeItem::insertChildren(int position, int count, int columns) {
    if (position < 0 || position > m_childItems.size())
        return false;
    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        TreeItem *item = new TreeItem(data, this);
        m_childItems.insert(position, item);
    }
    return true;
}


// метод ставит значение value в столбец column элемента:
bool TreeItem::setData(int column, const QVariant &value) {
    if (column < 0 || column >= m_itemData.size())
        return false;
    m_itemData[column] = value;
    return true;
}
