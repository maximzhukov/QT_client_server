#include "treeitem.h"
#include "model.h"
#include <QDebug>

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent) {
    QVector <QVariant> rootData;
    rootData << "Key" << "Value";
    rootItem = new TreeItem(rootData);
}

TreeModel::~TreeModel() { delete rootItem; }

int TreeModel::columnCount(const QModelIndex &parent) const {
    return rootItem->columnCount();
}

QVariant TreeModel::data (const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (role != Qt::DisplayRole  && role != Qt::EditRole)
        return QVariant();
    TreeItem *item = getItem(index);

    return item->data(index.column());
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const {
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);
    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
    TreeItem *parentItem = getItem(parent);
    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();
    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parentItem();
    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const {
    TreeItem *parentItem = getItem(parent);
    return parentItem->childCount();
}

void TreeModel::update(const QVector<Block>& blocks) {
    delete rootItem;
    QVector <QVariant> rootData;
    rootData << "Key" << "Value";
    rootItem = new TreeItem(rootData);
    setup(blocks);
}

void TreeModel::setup(const QVector<Block>& blocks) {
    TreeItem *parent = rootItem;
    QList<TreeItem*> parents;
    for (int block_index = 0; block_index < blocks.size(); ++block_index) {
        parents << parent;
        parents.last()->appendChild(new TreeItem({ "block id", blocks[block_index].id}, parents.last()));

        if (parents.last()->childCount() > 0) {
            parents << parents.last()->child(parents.last()->childCount()-1);
        }

        parents.last()->appendChild(new TreeItem({ "Name", blocks[block_index].Name}, parents.last()));
        parents.last()->appendChild(new TreeItem({ "BoardCount", blocks[block_index].BoardCount}, parents.last()));
        parents.last()->appendChild(new TreeItem({ "MtR", blocks[block_index].MtR}, parents.last()));
        parents.last()->appendChild(new TreeItem({ "MtC", blocks[block_index].MtC}, parents.last()));
        parents.last()->appendChild(new TreeItem({ "Description", blocks[block_index].Description}, parents.last()));
        parents.last()->appendChild(new TreeItem({ "Label", blocks[block_index].Label}, parents.last()));
        parents.last()->appendChild(new TreeItem({"boards"}, parents.last()));

        if (parents.last()->childCount() > 0) {
            parents << parents.last()->child(parents.last()->childCount()-1);
        }

        setup(blocks[block_index].boards, parents);

        if(parents.size() > 1) {
            parents.pop_back();
            parents.pop_back();
        }
    }
}

void TreeModel::setup(const QVector<Board>& boards, QList<TreeItem*> &parents) {
    for (int board_index = 0; board_index < boards.size(); ++board_index) {
        parents.last()->appendChild(new TreeItem({"board id", boards[board_index].id}, parents.last()));

        if (parents.last()->childCount() > 0) {
            parents << parents.last()->child(parents.last()->childCount()-1);
        }

        parents.last()->appendChild(new TreeItem({"Num"      ,  boards[board_index].Num,       }, parents.last()));
        parents.last()->appendChild(new TreeItem({"Name"     ,  boards[board_index].Name,      }, parents.last()));
        parents.last()->appendChild(new TreeItem({"PortCount",  boards[board_index].PortCount, }, parents.last()));
        parents.last()->appendChild(new TreeItem({"IntLinks" ,  boards[board_index].IntLinks,  }, parents.last()));
        parents.last()->appendChild(new TreeItem({"Algoritms",  boards[board_index].Algoritms, }, parents.last()));
        parents.last()->appendChild(new TreeItem({"ports"}, parents.last()));

        if (parents.last()->childCount() > 0) {
            parents << parents.last()->child(parents.last()->childCount()-1);
        }

        setup(boards[board_index].ports, parents);

        if(parents.size() > 1) {
            parents.pop_back();
            parents.pop_back();
        }
    }
}

void TreeModel::setup(const QVector<Port>& ports, QList<TreeItem*> &parents) {
    for (int port_index = 0; port_index < ports.size(); ++port_index) {
        parents.last()->appendChild(new TreeItem({"port id", ports[port_index].id}, parents.last()));

        if (parents.last()->childCount() > 0) {
            parents << parents.last()->child(parents.last()->childCount()-1);
        }

        parents.last()->appendChild(new TreeItem({"Num"   , ports[port_index].Num,    }, parents.last()));
        parents.last()->appendChild(new TreeItem({"Media" , ports[port_index].Media,  }, parents.last()));
        parents.last()->appendChild(new TreeItem({"Signal", ports[port_index].Signal, }, parents.last()));;

        if(parents.size() > 0)
            parents.pop_back();
    }
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole)
        return false;
    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);
    if (result)
        emit dataChanged(index, index);
    return result;
}

