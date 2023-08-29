#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QVector>

#include "PortModel.h"

struct Board {
    QString id, Num, Name, PortCount, IntLinks, Algoritms;
    QVector<Port> ports;
};

#endif // BOARDMODEL_H
