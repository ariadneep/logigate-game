
#include "level.h"
#include "wire.h"

enum class Component;
class Wire;

Level::Level(int width, QGraphicsScene* graphicsScene, b2World* box2DWorld, QObject *parent)
    : QObject{parent}, graphicsScene(graphicsScene), box2DWorld(box2DWorld), isVictory(false) {
    //levelNum = 0;
    confetti = new Confetti(graphicsScene, box2DWorld);
    // Initializes the grids to nullptrs.
    for (int i = 0; i < WIDTH * HEIGHT; i++)
        wireGrid[i] = nullptr;
}

Level::~Level() {
    for (int i = 0; i < WIDTH * HEIGHT; i++)
        delete wireGrid[i];
    delete confetti;
}

void Level::drawWire(int x, int y, QString tag) {

    qDebug() << "Checked at (" << x << ", " << y << ")";

    Wire* currentWire = getWire(x, y);

    if (currentWire == nullptr) {
        qDebug() << "empty space, attempting to place wire";

        Wire* upWire = getWire(x, y - 1);
        Wire* rightWire = getWire(x + 1, y);
        Wire* downWire = getWire(x, y + 1);
        Wire* leftWire = getWire(x - 1, y);

        // Check the above component:
        if (upWire != nullptr && upWire->getTag() == tag
            && !upWire->isFullyConnected()) {

            qDebug() << "Created new wire, connected to above.";
            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(upWire);
            currentWire->setDirection(Direction::NS);
            wireGrid[y * WIDTH + x] = currentWire;
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                    << " Tail: " << (currentWire->getTailConnection() != nullptr);

            // Alters the sprite of the head:
            if (upWire->getDirection() == Direction::EW)
                upWire->setDirection(Direction::SW);
        }

        // Check the right component:
        else if (rightWire != nullptr && rightWire->getTag() == tag
                   && !rightWire->isFullyConnected()) {

            qDebug() << "Created new wire, connected to right.";
            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(rightWire);
            currentWire->setDirection(Direction::EW);
            wireGrid[y * WIDTH + x] = currentWire;
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                     << " Tail: " << (currentWire->getTailConnection() != nullptr);

            if (rightWire->getDirection() == Direction::NS)
                rightWire->setDirection(Direction::NW);
        }

        // Check the below component:
        else if (downWire != nullptr && downWire->getTag() == tag
                   && !downWire->isFullyConnected()) {

            qDebug() << "Created new wire, connected to below.";
            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(downWire);
            currentWire->setDirection(Direction::NS);
            wireGrid[y * WIDTH + x] = currentWire;
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                     << " Tail: " << (currentWire->getTailConnection() != nullptr);
            if (downWire->getDirection() == Direction::EW)
                downWire->setDirection(Direction::NE);
        }

        // Check the left component:
        else if (leftWire != nullptr && leftWire->getTag() == tag
                   && !leftWire->isFullyConnected()) {

            qDebug() << "Created new wire, connected to left.";
            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(leftWire);
            currentWire->setDirection(Direction::EW);
            wireGrid[y * WIDTH + x] = currentWire;
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                     << " Tail: " << (currentWire->getTailConnection() != nullptr);
            if (leftWire->getDirection() == Direction::NS)
                leftWire->setDirection(Direction::NE);
        }

        // add checks for gates
    }
    else {

        // Is this wire directly connected to an incomplete end?
        // If so, "go back" one wire.
        if (currentWire->getTailConnection() == nullptr)
            currentWire->setTailConnection(nullptr);
    }

    qDebug() << "//////////////";
}

Wire* Level::getWire(int x, int y) {
    if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
        return nullptr;
    return wireGrid[y * WIDTH + x];
}

void Level::setWire(int x, int y, QString tag) {
    Wire* addWire = new Wire();
    addWire->setHeadConnection(addWire);
    addWire->setTag(tag);
    wireGrid[y * WIDTH + x] = addWire;
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
