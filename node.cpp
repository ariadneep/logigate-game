#include "node.h"

Node::Node(QObject *parent)
    : GridComponent{parent}
{
    signal = true;
    connected = false;
    // current code has been commented out because it used parameters that got deleted --
    // I'm not too sure how to make this working again without adding back some of the params

    // QString spritePath;
    // if (nodeType == NodeType::ROOT) {
    //     if(tag == "A") {
    //         spritePath = ":/sprites/Sprites/red_wires/red_button_down.png";
    //     }
    //     if(tag == "B") {
    //         //BLUE WIRE PATH HERE
    //     }
    // }



    /*
     * Add the sprite into the scene at the grid x and y-coordinates.
     */
    // sprite = new QGraphicsPixmapItem(QPixmap(spritePath));
    // sprite->setPos(xPos, yPos);
    // graphicsScene->addItem(sprite);
}

Node::~Node() {
    delete sprite;
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

NodeType Node::getNodeType() {
    return nodeType;
}

bool Node::checkSignal(QString& tag, bool signal) {
    if(nodeType != NodeType::END) {
        return false;
    }
    return (tag == this->tag && signal);
}

bool Node::getConnected() {
    return connected;
}

void Node::setConnected(bool connectionStatus) {
    connected = connectionStatus;
}
