
#include "gate.h"

Gate::Gate(int x, int y, Operator type, Alignment alignment, Direction direction, QObject *parent)
    : GridComponent{parent}, gateOperator(type), alignment(alignment), direction(direction) {
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

    // inputNode = new Node();
    // if(location == Gate::Location::NORTH)
    //     outputNode = new Node();
}

Gate::~Gate() {
    delete inputNode;
    delete outputNode;
}

Gate::Operator Gate::getOperator() {
    return gateOperator;
}

QString Gate::convertSignal(bool firstSignal, bool secondSignal, QString firstID, QString secondID) {
    bool newOutput;
    QString newID = firstID.append(secondID);
    //calls the specific helper method based on the Type
    switch(gateOperator) {
    case Operator::AND:
        newOutput = firstSignal && secondSignal;
        break;
    case Operator::OR:
        newOutput = firstSignal || secondSignal;
        break;
    default:
        return newID;
    }

    //sets outputSignal to the resulting boolean.
    outputSignal = newOutput;
    return newID;
}

QString Gate::convertSignal(bool input, QString id) {
    bool newOutput;

    //calls the specific helper method based on the Type
    switch(gateOperator) {
    case Operator::NOT:
        newOutput = !input;
        break;
    default:
        return id;
    }

    //sets outputSignal to the resulting boolean.
    outputSignal = newOutput;
    return id;
}

bool Gate::getSignal() {
    return outputSignal;
}

void Gate::setSignal(bool signal) {
    outputSignal = signal;
}

GridComponent::Type Gate::getType() {
    return GridComponent::Type::GATE;
}

void Gate::setOtherHalf(Gate* otherGate) {
    // Conditions wherein this method should do nothing.
    if(this->gateOperator == Operator::NOT || this->gateOperator != otherGate->gateOperator)
        return;

    // Connects this half of a two-square gate to the other half.
    otherHalf = otherGate;
}

Gate::Alignment Gate::getAlignment() {
    return alignment;
}

Gate::Direction Gate::getDirection() {
    return direction;
}

bool Gate::getIsInputConnected() {
    return inputNode->getConnected();
}

Node::Direction Gate::getInputDirection() {
    return inputNode->getDirection();
}

void Gate::connectWire(Wire* connectWire, Wire::Direction connectionDirection) {
    inputNode->setTag(connectWire->getTag());
    inputNode->connectWire(connectWire, connectionDirection);
}
