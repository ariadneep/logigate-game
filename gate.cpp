#include "gate.h"

Gate::Gate(Type type, QObject *parent)
    : GridComponent{parent}, gateType(type) {
    outputSignal = false;
}

Gate::~Gate() {

}

Type Gate::getType() {
    return gateType;
}

QString Gate::convertSignal(bool firstSignal, bool secondSignal, QString firstID, QString secondID) {
    bool newOutput;
    QString newID = firstID.append(secondID);
    //calls the specific helper method based on the Type
    switch(gateType) {
    case Type::AND:
        newOutput = firstSignal && secondSignal;
        break;
    case Type::OR:
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
    switch(gateType) {
    case Type::NOT:
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


