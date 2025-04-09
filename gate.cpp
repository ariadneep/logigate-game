#include "gate.h"

Gate::Gate(Operator type, QObject *parent)
    : GridComponent{parent}, gateOperator(type) {
    outputSignal = false;
}

Gate::~Gate() {

}

Operator Gate::getOperator() {
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


GridComponent::Type GridComponent::getType() {
    return GridComponent::Type::GATE;
}
