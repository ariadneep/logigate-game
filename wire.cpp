#include "wire.h"

Wire::Wire(QObject *parent)
    : GridComponent{parent}
{}

Wire::~Wire() {

}

GridComponent::Type Wire::getType() {
    return GridComponent::Type::WIRE;
}
