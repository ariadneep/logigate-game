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

void Gate::convertSignal(bool first, bool second) {
    bool newOutput;

    //calls the specific helper method based on the Type
    switch(gateType) {
    case Type::AND:
        newOutput = first && second;
        break;
    case Type::OR:
        newOutput = first || second;
        break;
    default:
        return;
    }

    //sets outputSignal to the resulting boolean.
    outputSignal = newOutput;
}

void Gate::convertSignal(bool input) {
    bool newOutput;

    //calls the specific helper method based on the Type
    switch(gateType) {
    case Type::NOT:
        newOutput = !input;
        break;
    default:
        return;
    }

    //sets outputSignal to the resulting boolean.
    outputSignal = newOutput;
}

bool Gate::getSignal() {
    return outputSignal;
}


