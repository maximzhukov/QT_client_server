#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVector>
#include <QVariant>

class TreeItem {
public:
    explicit TreeItem (const QVector<QVariant> &data,
                      TreeItem *parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);
    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int childNumber() const;

    TreeItem *parentItem();
    bool insertChildren(int position, int count, int columns);
    bool setData(int column, const QVariant &value);
private:
    QList <TreeItem*> m_childItems; //Список дочерних элементов
    QVector <QVariant> m_itemData; //Список данных текущего узла
    TreeItem *m_parentItem; //Ссылка на родительский узел
};
#endif // TREEITEM_H
