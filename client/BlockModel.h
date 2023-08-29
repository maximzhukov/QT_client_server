#ifndef BLOCKMODEL_H
#define BLOCKMODEL_H

#include <QVector>

#include "BoardModel.h"

struct Block {
    QString id, Name, Ip, BoardCount, MtR, MtC, Description, Label;
    QVector<Board> boards;
};

#endif // BLOCKMODEL_H
