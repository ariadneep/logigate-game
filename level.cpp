
#include "level.h"
#include "wire.h"

enum class Component;
class Wire;

Level::Level(int levelNum, QGraphicsScene* graphicsScene, b2World* box2DWorld, QObject *parent)
    : QObject{parent},box2DWorld(box2DWorld), graphicsScene(graphicsScene), isVictory(false) {

    confetti = new Confetti(graphicsScene, box2DWorld);
    // Initializes the grids to nullptrs.
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        wireGrid[i] = nullptr;
        gateGrid[i] = nullptr;
        nodeGrid[i] = nullptr;
        obstacleGrid[i] = nullptr;
    }

    //Setup the level based on the chosen level.
    levelSetup(levelNum);
}

Level::~Level() {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        delete wireGrid[i];
        delete gateGrid[i];
        delete nodeGrid[i];
        delete obstacleGrid[i];
    }
    delete confetti;
}

void Level::drawWire(int x, int y, QString tag) {

    qDebug() << "///////////\nChecked at (" << x << ", " << y << ")";

    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;

    Wire* currentWire = getWire(x, y);

    qDebug() << "currentWirePtr: " << currentWire;

    // nodeConnect(x, y, tag, currentNode);

    // Check for Nodes:

    if (currentWire == nullptr) {

        qDebug() << "empty space, attempting to place wire";

        Wire::Direction wireConnectionDirection = Wire::Direction::NONE;

        if (Node* connectNode = findNode(x, y, tag, wireConnectionDirection)) {

            currentWire = new Wire();
            setWire(x, y, currentWire);
            Wire* nodeWire = connectNode->getWire();
            // Check some stuff with the wire to make sure it is valid, namely,
            // are we connecting with a root or an end?

            switch (connectNode->getNodeType()) {
            case Node::Type::ROOT :
                connectWires(nodeWire, currentWire, wireConnectionDirection);
                break;
            case Node::Type::END :
                connectWires(currentWire, nodeWire, wireConnectionDirection);
                break;
            }

            // connectWires(nodeWire, currentWire, wireConnectionDirection);

            return;
        }
        else if (Wire* connectWire = findWire(x, y, tag, wireConnectionDirection)) {

            currentWire = new Wire();
            setWire(x, y, currentWire);
            connectWires(connectWire, currentWire, wireConnectionDirection);

            qDebug() << "Head: " << currentWire->getHeadConnection()
                     << " Tail: " << currentWire->getTailConnection();

            switch(wireConnectionDirection) {
            case Wire::Direction::N :

                // Draw NS Corner
                if (connectWire->getY() - connectWire->getHeadConnection()->getY() == 1) {
                    connectWire->setDirection(Wire::Direction::NS);
                }
                // Draw SE Corner
                else if (connectWire->getHeadConnection()->getX() - connectWire->getX() == 1) {
                    connectWire->setDirection(Wire::Direction::SE);
                }
                // Draw SW Corner
                // Refactor this later if no errors occurs
                else if (connectWire->getX() - connectWire->getHeadConnection()->getX() == 1) {
                    connectWire->setDirection(Wire::Direction::SW);
                }

                break;
            case Wire::Direction::E :

                // Draw NW Corner
                if (connectWire->getY() - connectWire->getHeadConnection()->getY() == 1) {
                    connectWire->setDirection(Wire::Direction::NW);
                }
                // Draw EW Corner
                else if (connectWire->getHeadConnection()->getX() - connectWire->getX() == 1) {
                    connectWire->setDirection(Wire::Direction::EW);
                }
                // Draw SW Corner
                // Refactor this later if no errors occurs
                else if (connectWire->getHeadConnection()->getY() - connectWire->getY() == 1) {
                    connectWire->setDirection(Wire::Direction::SW);
                }

                break;
            case Wire::Direction::S :

                // Draw NE Corner
                if (connectWire->getHeadConnection()->getX() - connectWire->getX() == 1) {
                    connectWire->setDirection(Wire::Direction::NE);
                }
                // Draw NS Corner
                else if (connectWire->getHeadConnection()->getY() - connectWire->getY() == 1) {
                    connectWire->setDirection(Wire::Direction::NS);
                }
                // Draw NW Corner
                // Refactor this later if no errors occurs
                else if (connectWire->getX() - connectWire->getHeadConnection()->getX() == 1) {
                    connectWire->setDirection(Wire::Direction::NW);
                }

                break;
            case Wire::Direction::W :

                // Draw NE Corner
                if (connectWire->getY() - connectWire->getHeadConnection()->getY() == 1) {
                    connectWire->setDirection(Wire::Direction::NE);
                }
                // Draw SE Corner
                else if (connectWire->getHeadConnection()->getY() - connectWire->getY() == 1) {
                    connectWire->setDirection(Wire::Direction::SE);
                }
                // Draw EW Corner
                // Refactor this later if no errors occurs
                else if (connectWire->getX() - connectWire->getHeadConnection()->getX() == 1) {
                    connectWire->setDirection(Wire::Direction::EW);
                }

                break;
            default :
                break;
            }
        }

        // wireConnect(x, y, tag, currentWire);

        // add checks for gates
    }
    else if (currentWire && currentWire->getTailConnection() != nullptr
             && currentWire->getTailConnection()->getTailConnection() == nullptr) {
        // Is this wire directly connected to an incomplete end?
        // If so, "go back" one wire.

        qDebug() << "deleted wire at " << x << ", " << y;

        wireRemove(currentWire);
    }

    qDebug() << "//////////////";
}

Wire* Level::findWire(int x, int y, QString tag, Wire::Direction& wireConnectionDirection) {

    Wire* upWire = getWire(x, y - 1);
    Wire* rightWire = getWire(x + 1, y);
    Wire* downWire = getWire(x, y + 1);
    Wire* leftWire = getWire(x - 1, y);

    // Check the above component:
    if (upWire != nullptr && upWire->getTag() == tag
        && !upWire->isFullyConnected()) {

        qDebug() << "Created new wire, connected to above.";
        wireConnectionDirection = Wire::Direction::N;
        return upWire;
    }

    // Check the right component:
    else if (rightWire != nullptr && rightWire->getTag() == tag
             && !rightWire->isFullyConnected()) {

        qDebug() << "Created new wire, connected to right.";
        wireConnectionDirection = Wire::Direction::E;
        return rightWire;
    }

    // Check the below component:
    else if (downWire != nullptr && downWire->getTag() == tag
             && !downWire->isFullyConnected()) {

        qDebug() << "Created new wire, connected to below.";
        wireConnectionDirection = Wire::Direction::S;
        return downWire;
    }

    // Check the left component:
    else if (leftWire != nullptr && leftWire->getTag() == tag
             && !leftWire->isFullyConnected()) {

        qDebug() << "Created new wire, connected to left.";
        wireConnectionDirection = Wire::Direction::W;
        return leftWire;
    }

    qDebug() << "No new wire. Return.";
    return nullptr;
}

void Level::wireRemove(Wire* currentWire) {
    int currentX = currentWire->getX();
    int currentY = currentWire->getY();
    int tailX = currentWire->getTailConnection()->getX();
    int tailY = currentWire->getTailConnection()->getY();
    int headX = currentWire->getHeadConnection()->getX();
    int headY = currentWire->getHeadConnection()->getY();

    setWire(tailX, tailY, nullptr);
    currentWire->setTailConnection(nullptr);

    // Draw N
    if (currentY - headY == 1) {
        currentWire->setDirection(Wire::Direction::N);
    }
    // Draw E
    else if (headX - currentX == 1) {
        currentWire->setDirection(Wire::Direction::E);
    }
    // Draw S
    else if (headY - currentY == 1) {
        currentWire->setDirection(Wire::Direction::S);
    }
    // Draw W
    // Refactor this later if no errors occurs
    else if (currentX - headX == 1) {
        currentWire->setDirection(Wire::Direction::W);
    }
}

Node* Level::findNode(int x, int y, QString tag, Wire::Direction& wireConnectionDirection) {

    Node* upNode = getNode(x, y - 1);
    Node* rightNode = getNode(x + 1, y);
    Node* downNode = getNode(x, y + 1);
    Node* leftNode = getNode(x - 1, y);

    if (upNode != nullptr && upNode->getTag() == tag && !upNode->getConnected()
        && (upNode->getDirection() == Node::Direction::NONE
            || upNode->getDirection() == Node::Direction::S)) {
        wireConnectionDirection = Wire::Direction::N;
        return upNode;
    }
    else if (rightNode != nullptr && rightNode->getTag() == tag && !rightNode->getConnected()
             && (rightNode->getDirection() == Node::Direction::NONE
                 || rightNode->getDirection() == Node::Direction::W)) {
        wireConnectionDirection = Wire::Direction::E;
        return rightNode;
    }
    else if (downNode != nullptr && downNode->getTag() == tag && !downNode->getConnected()
             && (downNode->getDirection() == Node::Direction::NONE
                 || downNode->getDirection() == Node::Direction::N)) {
        wireConnectionDirection = Wire::Direction::S;
        return downNode;
    }
    else if (leftNode != nullptr && leftNode->getTag() == tag && !leftNode->getConnected()
             && (leftNode->getDirection() == Node::Direction::NONE
                 || leftNode->getDirection() == Node::Direction::E)) {
        wireConnectionDirection = Wire::Direction::W;
        return leftNode;
    }
    qDebug() << "No node found";
    return nullptr;
}

void Level::connectWires(Wire* headWire, Wire* tailWire,
                        Wire::Direction& wireConnectionDirection) {

    tailWire->setTag(headWire->getTag());
    tailWire->setDirection(wireConnectionDirection);
    // PUT THIS IN THE DRAW WIRE METHOD INSTEAD AND THEN CONNECT.
    // setWire(x, y, tailWire);
    //
    headWire->setTailConnection(tailWire);
    tailWire->setHeadConnection(headWire);
    qDebug() << "Wires connected.";
}

Wire* Level::getWire(int x, int y) {
    if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT) {
        return nullptr;
    }
    return wireGrid[y * WIDTH + x];
}

void Level::setWire(int x, int y, Wire* newWire) {
    if (newWire)
        newWire->setPosition(x, y);
    wireGrid[y * WIDTH + x] = newWire;
}

Gate* Level::getGate(int x, int y) {
    if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT) {
        return nullptr;
    }
    return gateGrid[y * WIDTH + x];
}

Node* Level::getNode(int x, int y) {
    if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT) {
        return nullptr;
    }
    return nodeGrid[y * WIDTH + x];
}

Obstacle* Level::getObstacle(int x, int y) {
    if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT) {
        return nullptr;
    }
    return obstacleGrid[y * WIDTH + x];
}

void Level::setWireTemp(int x, int y, QString tag) {
    Wire* addWire = new Wire();
    addWire->setHeadConnection(addWire);
    addWire->setTag(tag);
    wireGrid[y * WIDTH + x] = addWire;
}

void Level::setNode(int x, int y, QString tag, Node::Type type) {
    Node* newNode = new Node(this, x, y, Node::Type::ROOT, tag);
    nodeGrid[y * WIDTH + x] = newNode;
}

void Level::victory() {
    /*
     * erm... should we change this to say "victoryCheck"?
     * probably
     */
    bool victory = true;
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        if (nodeGrid[i] != nullptr && nodeGrid[i]->getConnected() == false) {
            qDebug() << "was false!";
            victory = false;
        }
    }
    if(victory) {
        spawnConfetti();
        isVictory = true;
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

void Level::levelSetup(int levelNum) {
    /*
     * TODO: hardcoded grid values of where to place obstacles, nodes, and etc.
     */

    if (levelNum == 1) {
        /*
         * TODO: Determine what our level setup will be for each level and use
         * addObstacle, addNode, etc, to set up the level on the grid.
         */
    }

    if(levelNum == 2) {

    }

    if(levelNum == 3) {

    }

    if(levelNum == 4) {

    }

    if(levelNum == 5) {

    }
}

void Level::removeConfetti() {
    confetti->clearConfetti();
    isVictory = false;
}

void Level::clearLevel() {
    removeConfetti();

    for(int i = 0; i < WIDTH * HEIGHT; i++) {
        if (wireGrid[i]) {
            delete wireGrid[i];
            wireGrid[i] = nullptr;
        }

        if(gateGrid[i]) {
            delete gateGrid[i];
            gateGrid[i] = nullptr;
        }

        if(nodeGrid[i]) {
            delete nodeGrid[i];
            nodeGrid[i] = nullptr;
        }

        if(obstacleGrid[i]) {
            delete obstacleGrid[i];
            obstacleGrid[i] = nullptr;
        }
    }

    isVictory = false;
}


void Level::addGate(int x, int y, Operator gateType) {
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if(gateGrid[y * WIDTH + x] == nullptr) {
            gateGrid[y * WIDTH + x] = new Gate(gateType, this);
        }
    }
}

void Level::addNode(int x, int y, QString& tag, Node::Type nodeType, bool signal) {
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if(nodeGrid[y * WIDTH + x] == nullptr) {
            nodeGrid[y * WIDTH + x] = new Node(this);
            // TODO: refactor parameters;
            // nodeGrid[y * WIDTH + x] = new Node(tag, nodeType, graphicsScene, x, y, this);
            if(nodeType == Node::Type::ROOT) {
                nodeGrid[y * WIDTH + x]->setSignal(signal);
            }
        }
    }
}

void Level::addObstacle(int x, int y) {
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if(obstacleGrid[y * WIDTH + x] == nullptr) {
            obstacleGrid[y * WIDTH + x] = new Obstacle(this);
        }
    }
}
