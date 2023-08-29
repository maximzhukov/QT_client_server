#ifndef PORTMODEL_H
#define PORTMODEL_H

#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>

struct Port {
    QString id, Num, Media, Signal;
    QJsonObject to_json_object() const {
        QJsonObject obj;
        obj["id"] = id;
        obj["Num"] = Num;
        obj["Media"] = Media;
        obj["Signal"] = Signal;

        return obj;
    }
};

#endif // PORTMODEL_H
