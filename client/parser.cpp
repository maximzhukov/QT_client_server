#include "parser.h"

QVector<Block> Parser::parse_json(QString json_string) {
    QStringList propertyNames;
    QStringList propertyKeys;
    QJsonObject jsonObject = QJsonDocument::fromJson(json_string.toUtf8()).object();
    QJsonArray json_blocks = jsonObject["net"].toArray();

    QVector<Block> blocks;
    foreach (const QJsonValue & json_block, json_blocks) {
        QJsonObject obj = json_block.toObject();
        Block block;
        parse(block, obj);
        blocks.append(block);
    }

    return blocks;
}

void Parser::parse(Block& block, QJsonObject& object) {
    block.id = object["id"].toString();
    block.Name = object["Name"].toString();
    block.Ip = object["Ip"].toString();
    block.BoardCount = object["BoardCount"].toString();
    block.MtR = object["MtR"].toString();
    block.MtC = object["MtC"].toString();
    block.Description = object["Description"].toString();
    block.Label = object["Label"].toString();
    QJsonArray json_boards = object["boards"].toArray();

    foreach (const QJsonValue & json_board, json_boards) {
        QJsonObject obj = json_board.toObject();
        Board board;
        parse(board, obj);
        block.boards.append(board);
    }
}

void Parser::parse(Board& board, QJsonObject& object) {
    board.id = object["id"].toString();
    board.Num = object["Num"].toString();
    board.Name = object["Name"].toString();
    board.PortCount = object["PortCount"].toString();
    board.IntLinks = object["IntLinks"].toString();
    board.Algoritms = object["Algoritms"].toString();
    QJsonArray json_ports = object["ports"].toArray();

    foreach (const QJsonValue & json_port, json_ports) {
        QJsonObject obj = json_port.toObject();
        Port port;
        parse(port, obj);
        board.ports.append(port);
    }
}

void Parser::parse(Port& port, QJsonObject& object) {
    port.id = object["id"].toString();
    port.Num = object["Num"].toString();
    port.Media = object["Media"].toString();
    port.Signal = object["Signal"].toString();
}
