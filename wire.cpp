#include "wire.h"

class Node;

Wire::Wire(QObject *parent)
    : GridComponent{parent}
{
    headConnection = nullptr;
    tailConnection = nullptr;
    direction = Direction::NONE;

    // TODO: this signal means that the value the wire has is true
    // this might not always be true (i.e. if the wire starts from a
    // NOT gate
    signal = true;
    tag = "";
}

Wire::~Wire() {

}

bool Wire::getSignal() {
    return signal;
}

void Wire::setSignal(bool newSignal) {
    signal = newSignal;
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

Wire::Direction Wire::getDirection() {
    return direction;
}

void Wire::setDirection(Wire::Direction newDirection) {
    direction = newDirection;
}


