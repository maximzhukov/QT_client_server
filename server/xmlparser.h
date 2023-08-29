#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <QVector>

#include "BlockModel.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"

#define TEST_XML_PATH "./db_xml/"

class XmlParser
{
private:
    void set_attribute(const QString& attribute_name, QString& attribute, const boost::property_tree::ptree& node);
    bool parse(boost::property_tree::ptree const& node, Port& into);
    bool parse(boost::property_tree::ptree const& node, Board& into);
    bool parse(boost::property_tree::ptree const& node, Block& into);

    QVector<Block> blocks;
public:
    QVector<Block> parse_blocks(const QString& path);
};

#endif // XMLPARSER_H
