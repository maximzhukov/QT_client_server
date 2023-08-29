#ifndef TREEMODEL_H
#define TREEMODEL_H
#include <QAbstractItemModel>
#include <QModelIndex>
#include "treeitem.h"
#include "BlockModel.h"

class TreeModel : public QAbstractItemModel {
    Q_OBJECT
public:
    TreeModel(QObject *parent = 0);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void setupModelData(const QVector<Block>& blocks);
    void update(const QVector<Block>& blocks);
    TreeItem *rootItem; //ссылка на корневой узел
private:
    void setup(const QVector<Block>& blocks);
    void setup(const QVector<Board>& boards, QList<TreeItem*> &parents);
    void setup(const QVector<Port>& ports, QList<TreeItem*> &parents);
    TreeItem *getItem(const QModelIndex &index) const;
};
#endif // TREEMODEL_H
