#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDataStream>

#include "server.h"
#include "database.h"
#include "xmlparser.h"

#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

Server::Server() {
    if(QTcpServer::listen(QHostAddress::Any, 2332))
        qDebug() << "start listening";
    else
        qDebug() << "error connection to host";

    db.connectToDataBase();
    XmlParser parser;
    QVector<Block> data = parser.parse_blocks(TEST_XML_PATH);
    db.inserIntoTable(data);
    next_block_size = 0;
}

void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket();

    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    sockets.insert(socket->socketDescriptor(), socket);
    qDebug() << "socket connected on " << socketDescriptor << '\n';

}

void Server::slotReadyRead() {
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);
    if(in.status() == QDataStream::Ok) {
        QString str;
        in >> str;
        qDebug() << "read..";

        QVector<Block> fetched_data = db.fetch_data();
        QJsonObject obj;
        QJsonArray boards_array;
        foreach(const Block& block, fetched_data)
            boards_array.push_back(block.to_json_object());
        obj["net"] = boards_array;

        send_to_client(QJsonDocument(obj).toJson(QJsonDocument::Compact), socket->socketDescriptor());
    } else {
        qDebug() << "error read";
    }
}

void Server::send_to_client(QString str, qintptr descriptor) {
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15) ;
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));
    qDebug() << "socket->write " << data << " by socketDescriptor: " << sockets[descriptor]->socketDescriptor()  << '\n';
    sockets[descriptor]->write(data);
}
