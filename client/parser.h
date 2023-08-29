#ifndef PARSER_H
#define PARSER_H
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "BlockModel.h"

class Parser
{
public:
    QVector<Block> parse_json(QString json_string);
private:
    void parse(Block& block, QJsonObject& object);
    void parse(Board& board, QJsonObject& object);
    void parse(Port& port, QJsonObject& object);

};

#endif // PARSER_H
