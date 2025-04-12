#include "node.h"

Node::Node(QString& tag, NodeType nodeType, QObject *parent)
    : GridComponent{parent}, tag(tag), signal(false), nodeType(nodeType)
{}

Node::~Node() {

}

GridComponent::Type Node::getType() {
    return GridComponent::Type::NODE;
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

NodeType Node::getNodeType() {
    return nodeType;
}

bool Node::checkSignal(QString& tag, bool signal) {
    if(nodeType != NodeType::END) {
        return false;
    }

    return (tag == this->tag && signal);
}
