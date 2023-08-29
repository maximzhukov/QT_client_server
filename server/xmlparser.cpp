#include "xmlparser.h"
#include <QDebug>

void XmlParser::set_attribute(const QString& attribute_name, QString& attribute, const boost::property_tree::ptree& node) {
    try {
        attribute = QString::fromStdString(node.get<std::string>("<xmlattr>." + attribute_name.toStdString()));
    } catch (...) {
        attribute = "";
   }
}

bool XmlParser::parse(const boost::property_tree::ptree& node, Port& into) {
    set_attribute("id", into.id, node);
    set_attribute("Num", into.Num, node);
    set_attribute("Media", into.Media, node);
    set_attribute("Signal", into.Signal, node);

    return true;
}

bool XmlParser::parse(boost::property_tree::ptree const& node, Board& into) {
    set_attribute("id", into.id, node);
    set_attribute("Num", into.Num, node);
    set_attribute("Name", into.Name, node);
    set_attribute("PortCount", into.PortCount, node);
    set_attribute("IntLinks", into.IntLinks, node);
    set_attribute("Algoritms", into.Algoritms, node);

    for (auto& [name, child] : node) {
        if (name == "port") {
            into.ports.push_back(Port());
            if (!parse(child, into.ports.back())) {
                return false;
            }
        }
    }

    return true;
}

bool XmlParser::parse(boost::property_tree::ptree const& node, Block& into)  {
    set_attribute("id", into.id, node);
    set_attribute("Name", into.Name, node);
    set_attribute("BoardCount", into.BoardCount, node);
    set_attribute("MtR", into.MtR, node);
    set_attribute("MtC", into.MtC, node);
    set_attribute("Description", into.Description, node);
    set_attribute("Label", into.Label, node);

    for (auto& [name, child] : node) {
        if (name == "board") {
            into.boards.push_back(Board());
            if (!parse(child, into.boards.back())) {
                return false;
            }
        }
    }
    return true;
}

QVector<Block> XmlParser::parse_blocks(const QString& path) {
    blocks.clear();
    std::filesystem::directory_iterator iter;

    try {
        iter = std::filesystem::directory_iterator{path.toStdString()};
    } catch(...) {
        qDebug() << "ERROR: no directory " << path;
        return blocks;
    }

    for (const auto & entry : iter) {
        boost::property_tree::ptree ptreee;
        try {
            read_xml(entry.path().string(), ptreee);
        } catch (...) {
            qDebug() << "ERROR: read xml " << QString::fromStdString(entry.path().string());
            continue;
        }

        for (auto& [key, node] : ptreee.get_child("net")) {
            if ("block" == key) {
                Block block;
                parse(node, block);
                blocks.push_back(block);
            }
        }
    }
    return blocks;
}
