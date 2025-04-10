#include "level.h"

enum class Component;
class Wire;

Level::Level(int width, QGraphicsScene* graphicsScene, b2World* box2DWorld, QObject *parent)
    : QObject{parent}, graphicsScene(graphicsScene), box2DWorld(box2DWorld), isVictory(false) {
    //levelNum = 0;
    grid = new GridComponent[WIDTH * HEIGHT];
    confetti = new Confetti(graphicsScene, box2DWorld);
}

Level::~Level() {
    delete[] grid;
    delete confetti;
}

void Level::drawWire(int x, int y) {
    GridComponent* currentComponent = getGridComponent(x, y);
    GridComponent::Type currentType = currentComponent->getType();
}

GridComponent* Level::getGridComponent(int x, int y) {
    return &grid[y * WIDTH + x];
}

void Level::victory() {
    /*
     * TESTING PURPOSES, CHANGE LATER.
     */
    if(!isVictory) {
        isVictory = true;
        spawnConfetti();
    }
}

void Level::spawnConfetti() {
    confetti->spawnConfetti();
}

void Level::updateLevel() {
    if (isVictory) {
        confetti->updateConfetti();
    }
}

void Level::removeConfetti() {
    confetti->clearConfetti();
    isVictory = false;
}
