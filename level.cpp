#include "level.h"

enum class Component;
class Wire;

Level::Level(QObject *parent)
    : QObject{parent} {
    //levelNum = 0;
    grid = new GridComponent[WIDTH * HEIGHT];
}

Level::~Level() {
    delete[] grid;
}

void Level::drawWire(int x, int y) {
    GridComponent* currentComponent = getGridComponent(x, y);
    GridComponent::Type currentType = currentComponent->getType();
}

GridComponent* Level::getGridComponent(int x, int y) {
    return &grid[y * WIDTH * x];
}
