
#include "level.h"
#include "wire.h"

enum class Component;
class Wire;

Level::Level(QGraphicsScene* graphicsScene, b2World* box2DWorld, QObject *parent)
    : QObject{parent}, graphicsScene(graphicsScene), box2DWorld(box2DWorld), isVictory(false) {
    //levelNum = 0;
    confetti = new Confetti(graphicsScene, box2DWorld);
    // Initializes the grids to nullptrs.
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        wireGrid[i] = nullptr;
        nodeGrid[i] = nullptr;
    }
}

Level::~Level() {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        delete wireGrid[i];
        delete nodeGrid[i];
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

        qDebug() << "nullptr: " << nullptr;
        if (upWire) {
            qDebug() << "upwire fully connected: " << upWire->isFullyConnected();
            qDebug() << "upwire head: " << upWire->getHeadConnection();
            qDebug() << "upwire tail: " << upWire->getTailConnection();
            qDebug() << "tag: " << tag;
        }

        // Check the above component:
        if (upWire != nullptr && upWire->getTag() == tag
            && !upWire->isFullyConnected()) {

            qDebug() << "Created new wire, connected to above.";
            nodeCheck(x, y, upWire);
            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(upWire);
            currentWire->setDirection(Direction::NS);
            wireGrid[y * WIDTH + x] = currentWire;
            upWire->setTailConnection(currentWire);
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                    << " Tail: " << (currentWire->getTailConnection() != nullptr);

            // Alters the sprite of the head:
            if (upWire->getDirection() == Direction::EW)
                upWire->setDirection(Direction::SW);
        }

        // Check the right component:
        else if (rightWire != nullptr && rightWire->getTag() == tag
                   && !rightWire->isFullyConnected()) {

            nodeCheck(x, y, rightWire);
            qDebug() << "Created new wire, connected to right.";
            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(rightWire);
            currentWire->setDirection(Direction::EW);
            wireGrid[y * WIDTH + x] = currentWire;
            rightWire->setTailConnection(currentWire);
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                     << " Tail: " << (currentWire->getTailConnection() != nullptr);

            if (rightWire->getDirection() == Direction::NS)
                rightWire->setDirection(Direction::NW);
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
            currentWire->setDirection(Direction::NS);
            wireGrid[y * WIDTH + x] = currentWire;
            downWire->setTailConnection(currentWire);
            qDebug() << "Head: " << (currentWire->getHeadConnection() != nullptr)
                     << " Tail: " << (currentWire->getTailConnection() != nullptr);
            if (downWire->getDirection() == Direction::EW)
                downWire->setDirection(Direction::NE);
        }

        // Check the left component:
        else if (leftWire != nullptr && leftWire->getTag() == tag
                   && !leftWire->isFullyConnected()) {
            qDebug() << "Created new wire, connected to left.";
            nodeCheck(x, y, leftWire);


            currentWire = new Wire();
            currentWire->setTag(tag);
            currentWire->setHeadConnection(leftWire);
            currentWire->setDirection(Direction::EW);
            wireGrid[y * WIDTH + x] = currentWire;
            leftWire->setTailConnection(currentWire);
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

        // if (currentWire->getTailConnection() == nullptr)
        //     currentWire->setTailConnection(nullptr);
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

Wire* Level::getWire(int x, int y) {
    if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
        return nullptr;
    return wireGrid[y * WIDTH + x];
}

Node* Level::getNode(int x, int y) {
    if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
        return nullptr;
    return nodeGrid[y * WIDTH + x];
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

void Level::removeConfetti() {
    confetti->clearConfetti();
    isVictory = false;
}
