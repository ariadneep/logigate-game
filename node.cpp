#include "node.h"

Node::Node(QString& tag, NodeType nodeType, QGraphicsScene* graphicsScene, int xPos, int yPos, QObject *parent)
    : GridComponent{parent}, tag(tag), signal(false), nodeType(nodeType), sprite(nullptr), xPos(xPos), yPos(yPos)
{
    QString spritePath;
    if (nodeType == NodeType::ROOT) {
        if(tag == "A") {
            spritePath = ":/sprites/Sprites/red_wires/red_button_down.png";
        }
        if(tag == "B") {
            //BLUE WIRE PATH HERE
        }
    }

    /*
     * Add the sprite into the scene at the grid x and y-coordinates.
     */
    sprite = new QGraphicsPixmapItem(QPixmap(spritePath));
    sprite->setPos(xPos, yPos);
    graphicsScene->addItem(sprite);
}

Node::~Node() {
    delete sprite;
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
