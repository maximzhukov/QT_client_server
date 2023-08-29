#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "PortModel.h"

struct Board {
    QString id, Num, Name, PortCount, IntLinks, Algoritms;
    QVector<Port> ports;
    QJsonObject to_json_object() const {
        QJsonObject obj;
        obj["id"] = id;
        obj["Num"] = Num;
        obj["Name"] = Name;
        obj["PortCount"] = PortCount;
        obj["IntLinks"] = IntLinks;
        obj["Algoritms"] = Algoritms;

        QJsonArray ports_array;
        foreach(const Port& port, ports) {
            ports_array.push_back(port.to_json_object());
        }
        obj["ports"] = ports_array;

        return obj;
    };
};

#endif // BOARDMODEL_H
