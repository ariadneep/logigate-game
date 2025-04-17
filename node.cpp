#include "node.h"

Node::Node(QObject *parent, int x, int y, Node::Type type, QString tag)
    : GridComponent{parent}
{
    signal = true;
    connected = false;
    nodeType = type;
    this->tag = tag;
    this->direction = Node::Direction::NONE;
    backingWire = new Wire();
    backingWire->setTag(tag);
    backingWire->setPosition(x, y);

    switch (nodeType) {
    case Node::Type::ROOT :
        backingWire->setHeadConnection(backingWire);
        qDebug() << "From the constructor: ROOT";
        break;
    case Node::Type::END :
        qDebug() << "From the constructor: END";
        backingWire->setTailConnection(backingWire);
    }

    Node::Type result = this->getNodeType();
    if (result == Node::Type::ROOT) {
        qDebug() << "I'm da ROOT!";
    }
    else if (result == Node::Type::END) {
        qDebug() << "I'm da END!";
    }
}

Node::~Node() {
    delete sprite;
    delete backingWire;
}

GridComponent::Type Node::getType() {
    return GridComponent::Type::NODE;
}

void Node::setTag(QString newTag) {
    tag = newTag;
}

QString Node::getTag() {
    return tag;
}

bool Node::getSignal() {
    return signal;
}

void Node::setSignal(bool signal) {
    this->signal = signal;
}

Node::Type Node::getNodeType() {
    return nodeType;
}

Node::Direction Node::getDirection() {
    return direction;
}

void Node::setDirection(Node::Direction newDirection) {
    direction = newDirection;
}

bool Node::checkSignal(QString& tag, bool signal) {
    if(nodeType != Node::Type::END) {
        return false;
    }
    return (tag == this->tag && signal);
}

bool Node::getConnected() {
    return backingWire->isFullyConnected();
}

Wire* Node::getWire() {
    return backingWire;
}
