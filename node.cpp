#include "node.h"

Node::Node(QObject *parent)
    : GridComponent{parent}
{}

Node::~Node() {

}

GridComponent::Type Node::getType() {
    return GridComponent::Type::NODE;
}
