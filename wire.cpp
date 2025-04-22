#include "wire.h"
#include "qdebug.h"

class Node;

Wire::Wire(QObject *parent)
    : QObject{parent}
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
    headConnection = newWire;
}

Wire* Wire::getTailConnection() {
    return tailConnection;
}

void Wire::setTailConnection(Wire* newWire) {
    tailConnection = newWire;
}

Wire::Direction Wire::getDirection() {
    return direction;
}

void Wire::setDirection(Wire::Direction newDirection) {
    direction = newDirection;
}

int Wire::getX() {
    return x;
}

int Wire::getY() {
    return y;
}

void Wire::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void Wire::connectTail(Wire* connectWire, Wire::Direction connectionDirection) {
    if (connectWire == nullptr)
        return;

    connectWire->setDirection(connectionDirection);
    connectWire->setTag(tag);
    connectWire->setSignal(signal);
    setTailConnection(connectWire);
    connectWire->setHeadConnection(this);
    wireDualDirector(connectionDirection);
}

void Wire::wireDualDirector(Wire::Direction connectionDirection) {
    if (headConnection == nullptr)
        return;

    switch(connectionDirection) {
    case Wire::Direction::N :

        // Draw NS Corner
        if (y - headConnection->getY() == 1) {
            direction =  Wire::Direction::NS;
            return;
        }
        // Draw SE Corner
        else if (headConnection->getX() - x == 1) {
            direction = Wire::Direction::SE;
            return;
        }
        // Draw SW Corner
        // Refactor this later if no errors occurs
        else if (x - headConnection->getX() == 1) {
            direction = Wire::Direction::SW;
            return;
        }

        break;
    case Wire::Direction::E :

        // Draw NW Corner
        if (y - headConnection->getY() == 1) {
            direction = Wire::Direction::NW;
            return;
        }
        // Draw EW Corner
        else if (headConnection->getX() - x == 1) {
            direction = Wire::Direction::EW;
            return;
        }
        // Draw SW Corner
        // Refactor this later if no errors occurs
        else if (headConnection->getY() - y == 1) {
            direction = Wire::Direction::SW;
            return;
        }

        break;
    case Wire::Direction::S :

        // Draw NE Corner
        if (headConnection->getX() - x == 1) {
            direction = Wire::Direction::NE;
            return;
        }
        // Draw NS Corner
        else if (headConnection->getY() - y == 1) {
            direction = Wire::Direction::NS;
            return;
        }
        // Draw NW Corner
        // Refactor this later if no errors occurs
        else if (x - headConnection->getX() == 1) {
            direction = Wire::Direction::NW;
            return;
        }

        break;
    case Wire::Direction::W :

        qDebug() << "West Direction connection";
        // Draw NE Corner
        if (y - headConnection->getY() == 1) {
            direction = Wire::Direction::NE;
            return;
        }
        // Draw SE Corner
        else if (headConnection->getY() - y == 1) {
            direction = Wire::Direction::SE;
            return;
        }
        // Draw EW Corner
        // Refactor this later if no errors occurs
        else if (x - headConnection->getX() == 1) {
            direction = Wire::Direction::EW;
            return;
        }

        break;
    default : break;
    }
}


