#include "gate.h"

Gate::Gate(int x, int y, Operator type, Ports ports, Direction direction, QObject *parent)
    : QObject{parent}, gateOperator(type), alignment(ports), direction(direction) {
    outputSignal = false;
    otherHalf = nullptr;
    this->x = x;
    this->y = y;
    outputNode = nullptr;
    inputNode = nullptr;

    // Add a switch statement to accurately add Node pointers depending on orientation/type.
    inputNode = new Node(this, x, y, Node::Type::END, true, "");
    switch (direction) {
    case Gate::Direction::NORTH :
        inputNode->setDirection(Node::Direction::S);
        break;
    case Gate::Direction::EAST :
        inputNode->setDirection(Node::Direction::W);
        break;
    case Gate::Direction::SOUTH :
        inputNode->setDirection(Node::Direction::N);
        break;
    case Gate::Direction::WEST :
        inputNode->setDirection(Node::Direction::E);
        break;
    }
    if (ports == Ports::INOUT) {
        outputNode = new Node(this, x, y, Node::Type::ROOT, false, "");
        switch (direction) {
        case Gate::Direction::NORTH :
            outputNode->setDirection(Node::Direction::N);
            break;
        case Gate::Direction::EAST :
            outputNode->setDirection(Node::Direction::E);
            break;
        case Gate::Direction::SOUTH :
            outputNode->setDirection(Node::Direction::S);
            break;
        case Gate::Direction::WEST :
            outputNode->setDirection(Node::Direction::W);
            break;
        }
    }
}

Gate::~Gate() {
    delete inputNode;
    delete outputNode;
}

Gate::Operator Gate::getOperator() {
    return gateOperator;
}

void Gate::convertSignal(bool firstSignal, bool secondSignal, QString firstID, QString secondID) {
    bool newOutput;
    QString newID = firstID;
    newID.append(secondID);
    std::sort(newID.begin(), newID.end());

    //calls the specific helper method based on the Type
    switch(gateOperator) {
    case Operator::AND:
        newOutput = firstSignal && secondSignal;
        break;
    case Operator::OR:
        newOutput = firstSignal || secondSignal;
        break;
    default:
        return;
    }

    //sets outputSignal to the resulting boolean.
    if(outputNode) {
        outputNode->setSignal(newOutput);
        outputNode->setTag(newID);
    }
    else if (otherHalf->outputNode) {
        otherHalf->outputNode->setSignal(newOutput);
        otherHalf->outputNode->setTag(newID);
    }
}

void Gate::convertSignal(bool input, QString id) {
    if(gateOperator != Operator::NOT)
        return;
    bool newOutput = !input;

    //sets outputSignal to the resulting boolean.
    if(outputNode){
        outputNode->setSignal(newOutput);
        outputNode->setTag(id);
    }
}

bool Gate::getSignal() {
    return outputSignal;
}

void Gate::setSignal(bool signal) {
    outputSignal = signal;
}

Gate* Gate::getOtherHalf() {
    return otherHalf;
}

void Gate::setOtherHalf(Gate* otherGate) {
    // Conditions wherein this method should do nothing.
    if(this->gateOperator == Operator::NOT || this->gateOperator != otherGate->gateOperator)
        return;

    // Connects this half of a two-square gate to the other half.
    otherHalf = otherGate;
}

Gate::Ports Gate::getAlignment() {
    return alignment;
}

Gate::Direction Gate::getDirection() {
    return direction;
}

Node* Gate::getInputNode() {
    return inputNode;
}

bool Gate::isFullyConnected() {
    if(!otherHalf)
        return this->inputNode->getConnected();
    return this->inputNode->getConnected() && otherHalf->inputNode->getConnected();

}

Node::Direction Gate::getInputDirection() {
    return inputNode->getDirection();
}

QString Gate::getTag() {
    if (outputNode == nullptr && otherHalf->getOutputNode()) {
        return otherHalf->getOutputNode()->getTag();
    }
    else if (outputNode) {
        return outputNode->getTag();
    }
    return "";
}

Node* Gate::getOutputNode() {
    return outputNode;
}

Node::Direction Gate::getOutputDirection() {
    if (outputNode)
        return outputNode->getDirection();
    return Node::Direction::NONE;
}

void Gate::connectWire(Wire* connectWire, Wire::Direction connectionDirection) {
    //checks if this is an input-only wire
    if (alignment == Ports::IN && connectWire->getHeadConnection()) {
        inputNode->setTag(connectWire->getTag());
        inputNode->connectWire(connectWire, connectionDirection);
    }
    else {
        Node::Direction outputDirection = outputNode->getDirection();
        if ((connectionDirection == Wire::Direction::N && outputDirection == Node::Direction::S) ||
            (connectionDirection == Wire::Direction::E && outputDirection == Node::Direction::W) ||
            (connectionDirection == Wire::Direction::S && outputDirection == Node::Direction::N) ||
            (connectionDirection == Wire::Direction::W && outputDirection == Node::Direction::E)) {
            outputNode->setTag(getTag());
            outputNode->connectWire(connectWire, connectionDirection); //also sets signal
        }
        else if (connectWire->getHeadConnection()) {
            inputNode->setTag(connectWire->getTag());
            inputNode->connectWire(connectWire, connectionDirection);
        }
    }
    if(!connectWire->getHeadConnection())
        return;
    bool firstSignal = inputNode->getSignal();
    QString firstTag = inputNode->getTag();

    // If there is another half (ergo it's a double gate) run this
    if(otherHalf) {
        bool secondSignal = otherHalf->inputNode->getSignal();
        QString secondTag = otherHalf->inputNode->getTag();
        convertSignal(firstSignal, secondSignal, firstTag, secondTag);
        return;
    }

    // If there is no other half (ergo it's a single gate) run only this
    convertSignal(firstSignal, firstTag);
}
