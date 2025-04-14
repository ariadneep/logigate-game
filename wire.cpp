#include "wire.h"

Wire::Wire(QObject *parent)
    : GridComponent{parent}
{
    headConnection = nullptr;
    tailConnection = nullptr;
    direction = Direction::NONE;
    tag = "";
}

Wire::~Wire() {

}

QString Wire::getTag() {
    return tag;
}

void Wire::setTag(QString newTag) {
    tag = newTag;
}

bool Wire::isFullyConnected() {
    return (getHeadConnection() != nullptr && getTailConnection() != nullptr);
}

Wire* Wire::getHeadConnection() {
    return headConnection;
}

void Wire::setHeadConnection(Wire* newWire) {
    delete headConnection;
    headConnection = newWire;
}

Wire* Wire::getTailConnection() {
    return tailConnection;
}

void Wire::setTailConnection(Wire* newWire) {
    delete tailConnection;
    tailConnection = newWire;
}

Direction Wire::getDirection() {
    return direction;
}

void Wire::setDirection(Direction newDirection) {
    direction = newDirection;
}
