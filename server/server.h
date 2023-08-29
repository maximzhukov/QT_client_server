#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include "database.h"

class Server : public QTcpServer {
    Q_OBJECT
public:
    Server();
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
private:
    QMap<qintptr, QTcpSocket*> sockets;
    QByteArray data;
    void send_to_client(QString str, qintptr descriptor);
    DataBase db;
    quint16 next_block_size;
};

#endif // SERVER_H
