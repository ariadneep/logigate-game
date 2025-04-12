#include "obstacle.h"

Obstacle::Obstacle(QObject *parent)
    : GridComponent{parent}
{}

Obstacle::~Obstacle() {

}

GridComponent::Type Obstacle::getType() {
    return GridComponent::Type::OBSTACLE;
}
