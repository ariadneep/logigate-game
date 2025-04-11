#include "gridcomponent.h"

GridComponent::GridComponent(QObject *parent)
    : QObject{parent}
{}

GridComponent::~GridComponent() {}

GridComponent::Type GridComponent::getType() {
    return GridComponent::Type::EMPTY;
}