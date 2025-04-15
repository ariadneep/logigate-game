
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

    qDebug() << "Checked at (" << x << ", " << y << ")";

    Wire* currentWire = getWire(x, y);
    Node* currentNode = getNode(x, y);

    if (currentWire == nullptr && currentNode == nullptr) {

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
            nodeCheck(x, y, upWire);
            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(upWire);
            currentWire->setDirection(Wire::Direction::N);
            wireGrid[y * WIDTH + x] = currentWire;
            upWire->setTailConnection(currentWire);
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                    << " Tail: " << (currentWire->getTailConnection() != nullptr);

            // Alters the sprite of the head:
            switch (upWire->getDirection()) {
            case Wire::Direction::N :
                upWire->setDirection(Wire::Direction::NS);
                break;
            case Wire::Direction::W :
                upWire->setDirection(Wire::Direction::SW);
                break;
            case Wire::Direction::E :
                upWire->setDirection(Wire::Direction::SE);
                break;
            default:
                upWire->setDirection(Wire::Direction::NONE);
                break;
            }
        }

        // Check the right component:
        else if (rightWire != nullptr && rightWire->getTag() == tag
                   && !rightWire->isFullyConnected()) {

            nodeCheck(x, y, rightWire);
            qDebug() << "Created new wire, connected to right.";
            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(rightWire);
            currentWire->setDirection(Wire::Direction::E);
            wireGrid[y * WIDTH + x] = currentWire;
            rightWire->setTailConnection(currentWire);
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                     << " Tail: " << (currentWire->getTailConnection() != nullptr);

            switch (rightWire->getDirection()) {
            case Wire::Direction::N :
                rightWire->setDirection(Wire::Direction::NW);
                break;
            case Wire::Direction::E :
                rightWire->setDirection(Wire::Direction::EW);
                break;
            case Wire::Direction::S :
                rightWire->setDirection(Wire::Direction::SW);
                break;
            default:
                rightWire->setDirection(Wire::Direction::NONE);
                break;
            }
        }

        // Check the below component:
        else if (downWire != nullptr && downWire->getTag() == tag
                   && !downWire->isFullyConnected()) {

            // run node check, and see if there's a node to connect to.
            if(nodeCheck(x, y, downWire)){
                victory();
            }
            qDebug() << "Created new wire, connected to below.";
            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(downWire);
            currentWire->setDirection(Wire::Direction::S);
            wireGrid[y * WIDTH + x] = currentWire;
            downWire->setTailConnection(currentWire);
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                     << " Tail: " << (currentWire->getTailConnection() != nullptr);

            switch (downWire->getDirection()) {
            case Wire::Direction::E :
                downWire->setDirection(Wire::Direction::NE);
                break;
            case Wire::Direction::S :
                downWire->setDirection(Wire::Direction::NS);
                break;
            case Wire::Direction::W :
                downWire->setDirection(Wire::Direction::NW);
                break;
            default:
                downWire->setDirection(Wire::Direction::NONE);
                break;
            }
        }

        // Check the left component:
        else if (leftWire != nullptr && leftWire->getTag() == tag
                   && !leftWire->isFullyConnected()) {
            qDebug() << "Created new wire, connected to left.";
            nodeCheck(x, y, leftWire);


            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(leftWire);
            currentWire->setDirection(Wire::Direction::W);
            wireGrid[y * WIDTH + x] = currentWire;
            leftWire->setTailConnection(currentWire);
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                     << " Tail: " << (currentWire->getTailConnection() != nullptr);

            switch (leftWire->getDirection()) {
            case Wire::Direction::N :
                leftWire->setDirection(Wire::Direction::NE);
                break;
            case Wire::Direction::S :
                leftWire->setDirection(Wire::Direction::SE);
                break;
            case Wire::Direction::W :
                leftWire->setDirection(Wire::Direction::EW);
                break;
            default:
                leftWire->setDirection(Wire::Direction::NONE);
                break;
            }
        }
        else
            qDebug() << "No new wire. Return.";

        // add checks for gates
    }
    else {
        // Is this wire directly connected to an incomplete end?
        // If so, "go back" one wire.
        if (currentWire->getTailConnection()->getTailConnection() == nullptr) {
            qDebug() << "deleted wire at " << x << ", " << y;
            removeTail(x, y, currentWire);
        }
    }

    qDebug() << "//////////////";
}

bool Level::nodeCheck(int x, int y, Wire* currentWire) {
    Node* upNode = getNode(x, y - 1);
    Node* rightNode = getNode(x + 1, y);
    Node* downNode = getNode(x, y + 1);
    Node* leftNode = getNode(x - 1, y);

    QString currentTag = currentWire->getTag();
    bool currentSignal = currentWire->getSignal();
    bool wasChanged = false;

    if (upNode != nullptr && upNode->getTag() == currentTag && upNode->getSignal() == currentSignal) {
        qDebug() << "Node detected above, and meets all requirements to link!";
        upNode->setConnected(true);
        // TODO: ACTUALLY SET UP CONNECTION
        wasChanged = true;
    }
    else if (downNode != nullptr && downNode->getTag() == currentTag && downNode->getSignal() == currentSignal) {
        qDebug() << "Node detected below, and meets all requirements to link!";
        downNode->setConnected(true);
        // TODO: ACTUALLY SET UP CONNECTION
        wasChanged = true;
    }
    else if (rightNode != nullptr && rightNode->getTag() == currentTag && rightNode->getSignal() == currentSignal) {
        qDebug() << "Node detected to the right, and meets all requirements to link!";
        rightNode->setConnected(true);
        // TODO: ACTUALLY SET UP CONNECTION
        wasChanged = true;
    }
    else if (leftNode != nullptr && leftNode->getTag() == currentTag && leftNode->getSignal() == currentSignal) {
        qDebug() << "Node detected to the left, and meets all requirements to link!";
        leftNode->setConnected(true);
        // TODO: ACTUALLY SET UP CONNECTION
        wasChanged = true;
    }
    // return wasChanged -- meaning this method will return true if the wire happened to
    // connect to a node.
    return wasChanged;
 }

void Level::removeTail(int x, int y, Wire* currentWire) {
    Wire* markedWire = currentWire->getTailConnection();
    if (getWire(x, y - 1) == markedWire)
        wireGrid[(y - 1) * WIDTH + x] = nullptr;
    else if (getWire(x + 1, y) == markedWire)
        wireGrid[y * WIDTH + (x + 1)] = nullptr;
    else if (getWire(x, y + 1) == markedWire)
        wireGrid[(y + 1) * WIDTH + x] = nullptr;
    else if (getWire(x - 1, y) == markedWire)
        wireGrid[y * WIDTH + (x - 1)] = nullptr;

    currentWire->setTailConnection(nullptr);
}

Wire* Level::getWire(int x, int y) {
    if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT) {
        return nullptr;
    }
    qDebug() << "getWire done";
    return wireGrid[y * WIDTH + x];
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

void Level::setWire(int x, int y, QString tag) {
    Wire* addWire = new Wire();
    addWire->setHeadConnection(addWire);
    addWire->setTag(tag);
    wireGrid[y * WIDTH + x] = addWire;
}

void Level::setNode(int x, int y, QString tag) {
    Node* newNode = new Node();
    newNode->setTag(tag);
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
    /*
     * TODO: Change method to implement level-start up procedures such as
     * where nodes and wires will be in level.
     * Idea: Potentially delete method and move code into victory()?
     */
    if (isVictory) {
        confetti->updateConfetti();
    }
}

void Level::levelSetup(int levelNum) {
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

    /*
     * And ETC, based on how many levels we decide to have there will be more if-statements
     * with hardcoded grid values of where to place obstacles, nodes, and etc.
     */
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

void Level::addNode(int x, int y, QString& tag, NodeType nodeType, bool signal) {
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if(nodeGrid[y * WIDTH + x] == nullptr) {
            nodeGrid[y * WIDTH + x] = new Node(tag, nodeType, graphicsScene, x, y, this);
            if(nodeType == NodeType::ROOT) {
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
