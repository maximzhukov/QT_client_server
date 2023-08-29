#ifndef BLOCKMODEL_H
#define BLOCKMODEL_H

#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include "BoardModel.h"

struct Block {
    QString id, Name, Ip, BoardCount, MtR, MtC, Description, Label;
    QVector<Board> boards;

    QJsonObject to_json_object() const {
        QJsonObject obj;
        obj["id"] = id;
        obj["Name"] = Name;
        obj["BoardCount"] = BoardCount;
        obj["MtR"] = MtR;
        obj["MtC"] = MtC;
        obj["Description"] = Description;
        obj["Label"] = Label;

        QJsonArray boards_array;
        foreach(const Board& board, boards) {
            boards_array.push_back(board.to_json_object());
        }
        obj["boards"] = boards_array;

        return obj;
    }
};

#endif // BLOCKMODEL_H
