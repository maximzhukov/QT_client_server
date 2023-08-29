#include "database.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSqlRecord>

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}

void DataBase::connectToDataBase() {
    if(!QFile(DB_PATH "Blocks.db").exists()){
        this->createDataBase();
    } else {
        this->openDataBase();
        dropAll();
    }
}

void DataBase::dropAll() {
    QSqlQuery query;
    query.exec( "DROP TABLE IF EXISTS blocks;");
    query.exec( "DROP TABLE IF EXISTS boards;");
    query.exec( "DROP TABLE IF EXISTS ports;");
    this->createDataBase();
}

bool DataBase::createDataBase() {
    if(this->openDataBase()){
        return (this->createTable()) ? true : false;
    } else {
        qDebug() << "Не удалось создать базу данных";
        return false;
    }
    return false;
}

bool DataBase::openDataBase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("BlocksDB");
    db.setDatabaseName(DB_PATH "Blocks.db");
    if(db.open()){
        qDebug() << "DataBase: success open";
        return true;
    } else {
        qDebug() << "DataBase: can't open";
        return false;
    }
}

void DataBase::closeDataBase() { db.close(); }

bool DataBase::createTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE IF NOT EXISTS blocks ("
                    "   id TEXT PRIMARY KEY, "
                    "   Name TEXT,"
                    "   Ip Text, "
                    "   BoardCount TEXT, "
                    "   MtR TEXT, "
                    "   MtC TEXT, "
                    "   Description TEXT,"
                    "   Label TEXT)")) {
        qDebug() << "DataBase: error of create blocks";
        qDebug() << query.lastError().text();
        return false;
    }
    if(!query.exec("CREATE TABLE IF NOT EXISTS boards("
                    "    id TEXT PRIMARY KEY, "
                    "    block_id TEXT,"
                    "    Num TEXT,"
                    "    Name TEXT, "
                    "    PortCount TEXT, "
                    "    IntLinks TEXT, "
                    "    Algoritms TEXT,"
                    "    FOREIGN KEY (block_id)  REFERENCES blocks (id)"
                    "    )")) {
        qDebug() << "DataBase: error of create boards";
        qDebug() << query.lastError().text();
        return false;
    }
    if(!query.exec("CREATE TABLE IF NOT EXISTS ports("
                    "    id TEXT PRIMARY KEY, "
                    "    board_id TEXT,"
                    "    Num TEXT,"
                    "    Media TEXT, "
                    "    Signal TEXT,"
                    "    FOREIGN KEY (board_id)  REFERENCES boards (id)"
                    "    )")) {
        qDebug() << "DataBase: error of create ports";
        qDebug() << query.lastError().text();
        return false;
    }

    qDebug() << "DataBase: success of create TABLES";
    return true;
}

bool DataBase::inserIntoTable(const QVector<Block>& blocks)
{
    for (int i = 0; i < blocks.size(); ++i) {
        if(insert(blocks[i])) {
            for (int j = 0; j < blocks[i].boards.size(); ++j) {
                if(insert(blocks[i].boards[j], blocks[i].id)) {
                    for (int k = 0; k < blocks[i].boards[j].ports.size(); ++k) {
                        if(!insert(blocks[i].boards[j].ports[k], blocks[i].boards[j].id))
                            return false;
                    }
                } else {
                    return false;
                }
            }
        } else {
            return false;
        }
    }

    return true;
}

bool DataBase::insert(const Block& block) {
    QSqlQuery query;
    query.prepare("INSERT INTO blocks (id, Name, Ip, BoardCount, MtR, MtC, Description, Label)"
                  " VALUES (:id, :Name, :Ip, :BoardCount, :MtR, :MtC, :Description, :Label)");
    query.bindValue(":id", block.id);
    query.bindValue(":Name", block.Name);
    query.bindValue(":Ip", block.Ip);
    query.bindValue(":BoardCount", block.BoardCount);
    query.bindValue(":MtR", block.MtR);
    query.bindValue(":MtC", block.MtC);
    query.bindValue(":Description", block.MtC);
    query.bindValue(":Label", block.Label);

    if(!query.exec()){
        qDebug() << "error insert into Blocks";
        qDebug() << query.lastError().text();
        return false;
    } else {
        qDebug() << "Insert into blocks is success";
        return true;
    }
}

bool DataBase::insert(const Board& board, const QString& block_id) {
    QSqlQuery query;
    query.prepare("INSERT INTO boards (id, block_id, Num, Name, PortCount, IntLinks, Algoritms)"
                  "VALUES ("
                  ":id, "
                  ":block_id, "
                  ":Num, "
                  ":Name, "
                  ":PortCount, "
                  ":IntLinks, "
                  ":Algoritms)");
    query.bindValue(":id", board.id);
    query.bindValue(":block_id", block_id);
    query.bindValue(":Num", board.Num);
    query.bindValue(":Name", board.Name);
    query.bindValue(":PortCount", board.PortCount);
    query.bindValue(":IntLinks", board.IntLinks);
    query.bindValue(":Algoritms", board.Algoritms);

    if(!query.exec()){
        qDebug() << "error insert into Boards";
        qDebug() << query.lastError().text();
        return false;
    } else {
        qDebug() << "Insert into boards is success";
        return true;
    }
}

bool DataBase::insert(const Port& port, const QString& board_id) {
    QSqlQuery query;
    query.prepare("INSERT INTO ports (id, board_id, Num, Media, Signal)"
                  "VALUES ("
                  ":id, "
                  ":board_id, "
                  ":Num, "
                  ":Media, "
                  ":Signal)");
    query.bindValue(":id", port.id);
    query.bindValue(":board_id", board_id);
    query.bindValue(":Num", port.Num);
    query.bindValue(":Media", port.Media);
    query.bindValue(":Signal", port.Signal);

    if(!query.exec()){
        qDebug() << "error insert into Ports";
        qDebug() << query.lastError().text();
        return false;
    } else {
        qDebug() << "Insert into ports is success";
        return true;
    }
}

QVector<Block> DataBase::fetch_data() {
    QVector<Block> data;
    fetch(data);
    return data;
}

void DataBase::fetch(QVector<Block>& blocks) {
    QSqlQuery query;
    if (!query.exec("SELECT * FROM blocks"))
        qDebug() << "Error fetch from blocks";
    while(query.next()) {
        Block block;
        QSqlRecord localRecord = query.record();
        for (int i = 0; i < localRecord.count(); ++i) {
            if (localRecord.fieldName(i) == "id") {
                block.id = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "Name") {
                block.Name = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "Ip") {
                block.Ip = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "BoardCount") {
                block.BoardCount = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "MtR") {
                block.MtR = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "MtC") {
                block.MtC = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "Description") {
                block.Description = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "Label") {
                block.Label = query.value(i).toString();
            }
        }
        fetch(block.boards, block.id);
        blocks.push_back(block);
    }
}

void DataBase::fetch(QVector<Board>& boards, QString block_id) {
    QSqlQuery query;
    query.prepare("SELECT * FROM boards where block_id = (:block_id)");
    query.bindValue(":block_id", block_id);
    if (!query.exec())
        qDebug() << "Error fetch from boards";
    while(query.next()) {
        Board board;
        QSqlRecord localRecord = query.record();
        for (int i = 0; i < localRecord.count(); ++i) {
            if (localRecord.fieldName(i) == "id") {
                board.id = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "Num") {
                board.Num = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "Name") {
                board.Name = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "PortCount") {
                board.PortCount = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "IntLinks") {
                board.IntLinks = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "Algoritms") {
                board.Algoritms = query.value(i).toString();
            }
        }
        fetch(board.ports, board.id);
        boards.push_back(board);
    }
}

void DataBase::fetch(QVector<Port>& ports, QString board_id) {
    QSqlQuery query;
    query.prepare("SELECT * FROM ports where board_id = (:board_id)");
    query.bindValue(":board_id", board_id);
    if (!query.exec())
        qDebug() << "Error fetch from ports";
    while(query.next()) {
        Port port;
        QSqlRecord localRecord = query.record();
        for (int i = 0; i < localRecord.count(); ++i) {
            if (localRecord.fieldName(i) == "id") {
                port.id = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "Num") {
                port.Num = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "Media") {
                port.Media = query.value(i).toString();
            }
            if (localRecord.fieldName(i) == "Signal") {
                port.Signal = query.value(i).toString();
            }
        }
        ports.push_back(port);
    }
}
