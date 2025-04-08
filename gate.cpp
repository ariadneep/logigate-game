#include "gate.h"

Gate::Gate(Type type, QObject *parent)
    : GridComponent{parent}, gateType(type) {
    isDead = true;
    outputSignal = false;
}

Gate::~Gate() {

}

Type Gate::getType() {
    return gateType;
}

void Gate::convertSignal() {
    //calls the specific helper method based on the Type

    //sets outputSignal to the resulting boolean.
}

bool Gate::getSignal() {
    return outputSignal;
}


