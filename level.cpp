
#include "level.h"
#include "wire.h"

enum class Component;
class Wire;

Level::Level(QGraphicsScene* graphicsScene, b2World* box2DWorld, QObject *parent)
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

        if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
            return;

        qDebug() << "empty space, attempting to place wire";

        Wire* upWire = getWire(x, y - 1);
        Wire* rightWire = getWire(x + 1, y);
        Wire* downWire = getWire(x, y + 1);
        Wire* leftWire = getWire(x - 1, y);

        // Debugging:
        if (upWire != nullptr) {
            qDebug() << "upWire != nullptr passed";
            if (upWire->getTag() == tag) {
                qDebug() << "upWire->getTag() == tag passed";
            }
        }

        // Check the above component:
        if (upWire != nullptr && upWire->getTag() == tag
            && !upWire->isFullyConnected()) {

            qDebug() << "Created new wire, connected to above.";
            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(upWire);
            currentWire->setDirection(Wire::Direction::N);
            setWire(x, y, currentWire);
            upWire->setTailConnection(currentWire);
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                    << " Tail: " << (currentWire->getTailConnection() != nullptr);

            // Draw NS Corner
            if (upWire->getY() - upWire->getHeadConnection()->getY() == 1) {
                upWire->setDirection(Wire::Direction::NS);
            }
            // Draw SE Corner
            else if (upWire->getHeadConnection()->getX() - upWire->getX() == 1) {
                upWire->setDirection(Wire::Direction::SE);
            }
            // Draw SW Corner
            // Refactor this later if no errors occurs
            else if (upWire->getX() - upWire->getHeadConnection()->getX() == 1) {
                upWire->setDirection(Wire::Direction::SW);
            }
        }

        // Check the right component:
        else if (rightWire != nullptr && rightWire->getTag() == tag
                   && !rightWire->isFullyConnected()) {

            qDebug() << "Created new wire, connected to right.";
            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(rightWire);
            currentWire->setDirection(Wire::Direction::E);
            setWire(x, y, currentWire);
            rightWire->setTailConnection(currentWire);
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                     << " Tail: " << (currentWire->getTailConnection() != nullptr);

            // Draw NW Corner
            if (rightWire->getY() - rightWire->getHeadConnection()->getY() == 1) {
                rightWire->setDirection(Wire::Direction::NW);
            }
            // Draw EW Corner
            else if (rightWire->getHeadConnection()->getX() - rightWire->getX() == 1) {
                rightWire->setDirection(Wire::Direction::EW);
            }
            // Draw SW Corner
            // Refactor this later if no errors occurs
            else if (rightWire->getHeadConnection()->getY() - rightWire->getY() == 1) {
                rightWire->setDirection(Wire::Direction::SW);
            }
        }

        // Check the below component:
        else if (downWire != nullptr && downWire->getTag() == tag
                   && !downWire->isFullyConnected()) {

            qDebug() << "Created new wire, connected to below.";
            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(downWire);
            currentWire->setDirection(Wire::Direction::S);
            setWire(x, y, currentWire);
            downWire->setTailConnection(currentWire);
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                     << " Tail: " << (currentWire->getTailConnection() != nullptr);

            // Draw NE Corner
            if (downWire->getHeadConnection()->getX() - downWire->getX() == 1) {
                downWire->setDirection(Wire::Direction::NE);
            }
            // Draw NS Corner
            else if (downWire->getHeadConnection()->getY() - downWire->getY() == 1) {
                downWire->setDirection(Wire::Direction::NS);
            }
            // Draw NW Corner
            // Refactor this later if no errors occurs
            else if (downWire->getX() - downWire->getHeadConnection()->getX() == 1) {
                downWire->setDirection(Wire::Direction::NW);
            }
        }

        // Check the left component:
        else if (leftWire != nullptr && leftWire->getTag() == tag
                   && !leftWire->isFullyConnected()) {

            qDebug() << "Created new wire, connected to left.";
            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(leftWire);
            currentWire->setDirection(Wire::Direction::W);
            setWire(x, y, currentWire);
            leftWire->setTailConnection(currentWire);
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                     << " Tail: " << (currentWire->getTailConnection() != nullptr);

            // Draw NE Corner
            if (leftWire->getY() - leftWire->getHeadConnection()->getY() == 1) {
                leftWire->setDirection(Wire::Direction::NE);
            }
            // Draw SE Corner
            else if (leftWire->getHeadConnection()->getY() - leftWire->getY() == 1) {
                leftWire->setDirection(Wire::Direction::SE);
            }
            // Draw EW Corner
            // Refactor this later if no errors occurs
            else if (leftWire->getX() - leftWire->getHeadConnection()->getX() == 1) {
                leftWire->setDirection(Wire::Direction::EW);
            }
        }
        else
            qDebug() << "No new wire. Return.";

        // add checks for gates
    }
    else {

        // Is this wire directly connected to an incomplete end?
        // If so, "go back" one wire.
        if (currentWire->getTailConnection() != nullptr
            && currentWire->getTailConnection()->getTailConnection() == nullptr) {
            qDebug() << "deleted wire at " << x << ", " << y;
            int tailX = currentWire->getTailConnection()->getX();
            int tailY = currentWire->getTailConnection()->getY();
            setWire(tailX, tailY, nullptr);
            currentWire->setTailConnection(nullptr);

            // Draw N
            if (currentWire->getY() - currentWire->getHeadConnection()->getY() == 1) {
                currentWire->setDirection(Wire::Direction::N);
            }
            // Draw E
            else if (currentWire->getHeadConnection()->getX() - currentWire->getX() == 1) {
                currentWire->setDirection(Wire::Direction::E);
            }
            // Draw S
            else if (currentWire->getHeadConnection()->getY() - currentWire->getY() == 1) {
                currentWire->setDirection(Wire::Direction::S);
            }
            // Draw W
            // Refactor this later if no errors occurs
            else if (currentWire->getX() - currentWire->getHeadConnection()->getX() == 1) {
                currentWire->setDirection(Wire::Direction::W);
            }
        }
    }

    qDebug() << "//////////////";
}

Wire* Level::getWire(int x, int y) {
    if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
        return nullptr;
    qDebug() << "getWire done";
    return wireGrid[y * WIDTH + x];
}

void Level::setWire(int x, int y, Wire* newWire) {
    if (newWire)
        newWire->setPosition(x, y);
    wireGrid[y * WIDTH + x] = newWire;
}

Gate* Level::getGate(int x, int y) {
    return nullptr;
}

Node* Level::getNode(int x, int y) {
    return nullptr;
}

Obstacle* Level::getObstacle(int x, int y) {
    return nullptr;
}

void Level::setWireTemp(int x, int y, QString tag) {
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
