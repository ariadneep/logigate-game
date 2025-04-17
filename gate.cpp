
#include "gate.h"

Gate::Gate(Operator type, QObject *parent)
    : GridComponent{parent}, gateOperator(type) {
    outputSignal = false;
    tag = "";
    otherHalf = nullptr;

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

QString Gate::getGateTag() {
    return tag;
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
