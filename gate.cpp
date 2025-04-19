
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

    // CHANGE THE AB TAG, JUST FOR TESTING
    if (alignment == Alignment::SECOND) {
        outputNode = new Node(this, x, y, Node::Type::ROOT, false, "AB");
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
    qDebug() << "Gate signal conversion = " << newOutput;
    qDebug() << "Gate signal tag = " << outputNode->getTag();
    outputNode->setSignal(newOutput);
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
    outputNode->setSignal(newOutput);
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

Node* Gate::getInputNode() {
    return inputNode;
}

bool Gate::getIsInputConnected() {
    return inputNode->getConnected();
}

bool Gate::getIsBothInputsConnected() {
    return inputNode->getConnected() && otherHalf->getIsInputConnected();
}

Node::Direction Gate::getInputDirection() {
    return inputNode->getDirection();
}

QString Gate::getTag() {
    // Change this later?
    if (outputNode == nullptr && otherHalf->getOutputNode()) {
        qDebug() << "getTag branch 1: " << otherHalf->getOutputNode()->getTag();
        return otherHalf->getOutputNode()->getTag();
    }
    else if (outputNode) {
        qDebug() << "getTag branch 2: " << outputNode->getTag();
        return outputNode->getTag();
    }
    qDebug() << "getTag branch return";
    return "";
    // return outputTag;
}

Node* Gate::getOutputNode() {
    return outputNode;
}

Wire* Gate::getOutputWire() {
    return outputNode->getWire();
}

Node::Direction Gate::getOutputDirection() {
    if (outputNode)
        return outputNode->getDirection();
    return Node::Direction::NONE;
}

void Gate::connectWire(Wire* connectWire, Wire::Direction connectionDirection) {
    if (alignment == Alignment::FIRST && connectWire->getHeadConnection()) {
        inputNode->setTag(connectWire->getTag());
        inputNode->connectWire(connectWire, connectionDirection);
        if (gateOperator == Gate::Operator::NOT) {
            convertSignal(inputNode->getSignal(), inputNode->getTag());
        }
    }
    else {
        Node::Direction outputDirection = outputNode->getDirection();
        // DO A DIRECTION CHECK:
        if ((connectionDirection == Wire::Direction::N && outputDirection == Node::Direction::S) ||
            (connectionDirection == Wire::Direction::E && outputDirection == Node::Direction::W) ||
            (connectionDirection == Wire::Direction::S && outputDirection == Node::Direction::N) ||
            (connectionDirection == Wire::Direction::W && outputDirection == Node::Direction::E)) {

            outputNode->setSignal(outputSignal);
            outputNode->setTag(getTag());
            outputNode->connectWire(connectWire, connectionDirection);
        }
        else if (connectWire->getHeadConnection()) {
            inputNode->setTag(connectWire->getTag());
            inputNode->connectWire(connectWire, connectionDirection);

            bool firstSignal = inputNode->getSignal();
            QString firstTag = inputNode->getTag();
            bool secondSignal = otherHalf->getSignal();
            QString secondTag = otherHalf->getTag();
            convertSignal(firstSignal, secondSignal, firstTag, secondTag);
        }
    }
}
