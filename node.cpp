#include "node.h"

Node::Node(QObject *parent, int x, int y, Node::Type type, bool signal, QString tag)
    : QObject{parent}
{
    this->signal = signal;
    nodeType = type;
    this->tag = tag;
    this->direction = Node::Direction::NONE;
    backingWire = new Wire();
    backingWire->setTag(tag);
    backingWire->setSignal(signal);
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
    //delete sprite;
    delete backingWire;
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
    qDebug() << "current signal: " << signal;
    this->signal = signal;
    qDebug() << "ran correctly";
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

void Node::connectWire(Wire* connectWire, Wire::Direction nodeConnectionDirection) {
    if (connectWire == nullptr)
        return;

    switch (nodeType) {
    case Node::Type::ROOT :
        // connectWire->setDirection(connectionDirection);
        backingWire->connectTail(connectWire, nodeConnectionDirection);
        break;
    case Node::Type::END :
        qDebug() << "End Node found";

        // Flip the direction:
        switch (nodeConnectionDirection) {
        case Wire::Direction::N :
            nodeConnectionDirection = Wire::Direction::S;
            break;
        case Wire::Direction::E :
            nodeConnectionDirection = Wire::Direction::W;
            break;
        case Wire::Direction::S :
            nodeConnectionDirection = Wire::Direction::N;
            break;
        case Wire::Direction::W :
            nodeConnectionDirection = Wire::Direction::E;
            break;
        default : break;
        }

        // Connect the head to the tail node:
        connectWire->connectTail(backingWire, nodeConnectionDirection);
        break;
    }
}
