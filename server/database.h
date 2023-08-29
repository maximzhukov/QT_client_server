#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QVector>

#include "BlockModel.h"

#define DB_PATH "./"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    void connectToDataBase();

private:
    QSqlDatabase db;

private:
    bool insert(const Block& block);
    bool insert(const Board& board, const QString& blockid);
    bool insert(const Port& port, const QString& boardid);
    bool openDataBase();
    bool createDataBase();
    void closeDataBase();
    bool createTable();
    void fetch(QVector<Port>& ports, QString boardid);
    void fetch(QVector<Board>& boards, QString blockid);
    void fetch(QVector<Block>& blocks);
public slots:
    void dropAll();
    bool inserIntoTable(const QVector<Block>& blocks);
    QVector<Block> fetch_data();
};

#endif // DATABASE_H
