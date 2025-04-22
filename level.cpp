
#include "level.h"
#include "wire.h"

enum class Component;
class Wire;

Level::Level(QGraphicsScene* graphicsScene, b2World* box2DWorld, QObject *parent)
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
    // levelSetup(levelNum);
}

Level::~Level() {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        delete wireGrid[i];
        delete gateGrid[i];
        delete nodeGrid[i];
        delete obstacleGrid[i];
    }
    if (confetti) {
        delete confetti;
    }
}

void Level::drawWire(int x, int y, QString tag) {

    qDebug() << "///////////\nChecked at (" << x << ", " << y << ")";

    if ((x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        || tag == "") {
        qDebug() << "Draw wire skipped. (tag: " << tag << ")";
        return;
    }

    Wire* currentWire = getWire(x, y);

    if (isEmptySpace(x, y)) {


        Wire::Direction wireConnectionDirection = Wire::Direction::NONE;
        Wire* connectWire = findWire(x, y, tag, wireConnectionDirection);

        qDebug() << "empty space, attempting to place wire";

        Wire* currentWire = nullptr;
        Wire::Direction nodeConnectionDirection = Wire::Direction::NONE;

        // Attempting to find valid neighboring node:
        if (Node* connectNode = findNode(x, y, tag, nodeConnectionDirection, connectWire)) {
            // Check some stuff with the wire to make sure it is valid, namely,
            // are we connecting with a root or an end?

            qDebug() << "Node found";

            currentWire = new Wire();
            setWire(x, y, currentWire);
            // Checks if there's a head wire and sets it to current node. For the end Node.
            if (connectWire) {
                connectWire->connectTail(currentWire, wireConnectionDirection);
            }
            connectNode->connectWire(currentWire, nodeConnectionDirection);

            return;
        }

        // Attempting to find neighboring valid gate:
        else if (Gate* connectGate = findGate(x, y, nodeConnectionDirection, connectWire)) {
            qDebug() << "Gate found";

            currentWire = new Wire();
            currentWire->setTag(tag);
            setWire(x, y, currentWire);
            // Checks if there's a head wire and sets it to current node. For the end Node.
            if (connectWire) {
                connectWire->connectTail(currentWire, wireConnectionDirection);
            }
            connectGate->connectWire(currentWire, nodeConnectionDirection);

            return;
        }

        // Attempting to find neighboring valid wire:
        else if (connectWire) {

            qDebug() << "Attempting to create a new wire.";
            currentWire = new Wire();
            setWire(x, y, currentWire);
            connectWire->connectTail(currentWire, wireConnectionDirection);
        }
    }
    else if (currentWire && currentWire->getTailConnection() != nullptr
             && currentWire->getTailConnection()->getTailConnection() == nullptr) {
        // Is this wire directly connected to an incomplete end?
        // If so, "go back" one wire.

        qDebug() << "deleted wire at " << x << ", " << y;

        wireRemove(currentWire);
    }

    qDebug() << "//////////////";

    victory();
}

Wire* Level::findWire(int x, int y, QString tag, Wire::Direction& wireConnectionDirection) {

    qDebug() << "In find wire";
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

    qDebug() << "No wire link found. Return.";
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
    // currentWire->setTailConnection(nullptr);

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

Node* Level::findNode(int x, int y, QString tag, Wire::Direction& wireConnectionDirection, Wire* connectWire) {

    qDebug() << "In find node";
    Node* upNode = getNode(x, y - 1);
    Node* rightNode = getNode(x + 1, y);
    Node* downNode = getNode(x, y + 1);
    Node* leftNode = getNode(x - 1, y);
    qDebug() << "passed pointer collection";
    qDebug() << "upNode " << upNode;
    qDebug() << "rightNode " << rightNode;
    qDebug() << "downNode " << downNode;
    qDebug() << "leftNode " << leftNode;

    if (upNode != nullptr && upNode->getTag() == tag && !upNode->getConnected()
        && (upNode->getDirection() == Node::Direction::NONE
        || upNode->getDirection() == Node::Direction::S) &&
        (upNode->getNodeType() == Node::Type::ROOT ||
        (connectWire && upNode->getSignal() == connectWire->getSignal()))) {
        wireConnectionDirection = Wire::Direction::N;
        return upNode;
    }
    else if (rightNode != nullptr && rightNode->getTag() == tag && !rightNode->getConnected()
            && (rightNode->getDirection() == Node::Direction::NONE
            || rightNode->getDirection() == Node::Direction::W) &&
            (rightNode->getNodeType() == Node::Type::ROOT ||
            (connectWire && rightNode->getSignal() == connectWire->getSignal()))) {
        wireConnectionDirection = Wire::Direction::E;
        return rightNode;
    }
    else if (downNode != nullptr && downNode->getTag() == tag && !downNode->getConnected()
            && (downNode->getDirection() == Node::Direction::NONE
            || downNode->getDirection() == Node::Direction::N) &&
            (downNode->getNodeType() == Node::Type::ROOT ||
            (connectWire && downNode->getSignal() == connectWire->getSignal()))) {
        wireConnectionDirection = Wire::Direction::S;
        return downNode;
    }
    else if (leftNode != nullptr && leftNode->getTag() == tag && !leftNode->getConnected()
            && (leftNode->getDirection() == Node::Direction::NONE
            || leftNode->getDirection() == Node::Direction::E) &&
            (leftNode->getNodeType() == Node::Type::ROOT ||
            (connectWire && leftNode->getSignal() == connectWire->getSignal()))) {
        wireConnectionDirection = Wire::Direction::W;
        return leftNode;
    }
    qDebug() << "No node found";
    return nullptr;
}

Gate* Level::findGate(int x, int y, Wire::Direction& wireConnectionDirection, Wire* headWire) {

    qDebug() << "In find gate";
    Gate* upGate = getGate(x, y - 1);
    Gate* rightGate = getGate(x + 1, y);
    Gate* downGate = getGate(x, y + 1);
    Gate* leftGate = getGate(x - 1, y);

    qDebug() << "upGate: " << upGate;
    qDebug() << "rightGate: " << rightGate;
    qDebug() << "downGate: " << downGate;
    qDebug() << "leftGate: " << leftGate;

    // Check gates above:
    if (upGate &&
        ((upGate->getInputDirection() == Node::Direction::S && headWire) ||
        (upGate->getOutputDirection() == Node::Direction::S
        && upGate->isFullyConnected()))) {
        wireConnectionDirection = Wire::Direction::N;
        return upGate;
    }

    // Check gates to the right:
    else if (rightGate &&
            ((rightGate->getInputDirection() == Node::Direction::W && headWire) ||
            (rightGate->getOutputDirection() == Node::Direction::W
            && rightGate->isFullyConnected()))) {
        wireConnectionDirection = Wire::Direction::E;
        return rightGate;
    }

    // Check gates below:
    else if (downGate &&
            ((downGate->getInputDirection() == Node::Direction::N && headWire) ||
            (downGate->getOutputDirection() == Node::Direction::N
            && downGate->isFullyConnected()))) {
        wireConnectionDirection = Wire::Direction::S;
        return downGate;
    }

    // Check gates left:
    else if (leftGate &&
            ((leftGate->getInputDirection() == Node::Direction::E && headWire) ||
            (leftGate->getOutputDirection() == Node::Direction::E
            && leftGate->isFullyConnected()))) {
        wireConnectionDirection = Wire::Direction::W;
        return leftGate;
    }
    qDebug() << "No gate found";
    return nullptr;
}

Wire* Level::getWire(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return nullptr;
    }
    return wireGrid[y * WIDTH + x];
}

void Level::setWire(int x, int y, Wire* newWire) {
    if ((x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
        || getNode(x, y) || getObstacle(x, y) || getGate(x, y)) {
        return;
    }

    if (wireGrid[y * WIDTH + x]) {
        if (Wire* headWire = wireGrid[y * WIDTH + x]->getHeadConnection())
            headWire->setTailConnection(newWire);
        if (Wire* tailWire = wireGrid[y * WIDTH + x]->getTailConnection())
            tailWire->setHeadConnection(newWire);

        delete wireGrid[y * WIDTH + x];
    }

    if (newWire)
        newWire->setPosition(x, y);

    wireGrid[y * WIDTH + x] = newWire;
}

Gate* Level::getGate(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return nullptr;
    }
    return gateGrid[y * WIDTH + x];
}

Node* Level::getNode(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return nullptr;
    }
    return nodeGrid[y * WIDTH + x];
}

Obstacle* Level::getObstacle(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
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

void Level::setNode(int x, int y, bool signal, QString tag, Node::Type type) {

    if (getGate(x, y) || getWire(x, y) || getNode(x, y) || getObstacle(x, y))
        return;


    Node* newNode = new Node(this, x, y, type, signal, tag);
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
    if(victory && !isVictory) {
        spawnConfetti();
        isVictory = true;
        emit levelCompleted();

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

    // clearLevel();

    switch (levelNum) {
    case 1:
        setNode(0, 0, true, "A", Node::Type::ROOT);
        setNode(0, 7, true, "B", Node::Type::ROOT);
        for(int x = 1; x < 12; x++) {
            for(int y = 0; y < 3; y++) {
                addObstacle(x, y);
            }
        }

        for(int x = 1; x < 12; x++) {
            for(int y = 5; y < 8; y++) {
                addObstacle(x, y);
            }
        }

        drawGate(6, 4, Gate::Operator::AND, Gate::Direction::EAST);
        setNode(11, 4, true, "AB", Node::Type::END);
        break;
        break;

    case 2:
        setNode(2, 2, false, "A", Node::Type::ROOT);
        drawGate(4 ,3, Gate::Operator::NOT, Gate::Direction::SOUTH);
        setNode(2, 5, true, "B", Node::Type::ROOT);
        addObstacle(6,2);
        addObstacle(7,2);
        addObstacle(8,2);
        addObstacle(9,2);
        addObstacle(6,3);
        addObstacle(7,3);
        addObstacle(8,3);
        addObstacle(9,3);
        // Top two rows
        for(int x = 0; x < 12; x++) {
            for(int y = 0; y < 2; y++) {
                addObstacle(x, y);
            }
        }
        // Bottom Row
        for(int x = 0; x < 12; x++) {
            for(int y = 6; y < 8; y++) {
                addObstacle(x, y);
            }
        }
        // Left side
        for(int x = 0; x < 2; x++) {
            for(int y = 0; y < 7; y++) {
                addObstacle(x, y);
            }
        }
        // Right side
        for(int x = 10; x < 12; x++) {
            for(int y = 0; y < 7; y++) {
                addObstacle(x, y);
            }
        }

        drawGate(6, 5, Gate::Operator::AND, Gate::Direction::EAST);
        setNode(9, 5, true, "AB", Node::Type::END);

        break;

    case 3:
        // Top Row
        for(int x = 0; x < 12; x++) {
            for(int y = 0; y < 2; y++) {
                addObstacle(x, y);
            }
        }
        // Left Side
        for(int x = 0; x < 2; x++) {
            for(int y = 0; y < 8; y++) {
                addObstacle(x, y);
            }
        }

        // Right side
        for(int x = 10; x < 12; x++) {
            for(int y = 0; y < 8; y++) {
                addObstacle(x, y);
            }
        }

        addObstacle(8, 5);
        addObstacle(8, 6);
        addObstacle(8, 4);
        addObstacle(8, 7);

        setNode(6, 7, false, "B", Node::Type::ROOT);
        setNode(7, 2, true, "A", Node::Type::ROOT);
        setNode(9, 7, true, "AB", Node::Type::END);

        drawGate(3, 5, Gate::Operator::OR, Gate::Direction::EAST);
        drawGate(7, 5, Gate::Operator::NOT, Gate::Direction::NORTH);

        break;

    case 4:
        // Top two rows
        for(int x = 0; x < 12; x++) {
            for(int y = 0; y < 2; y++) {
                addObstacle(x, y);
            }
        }
        // Bottom Row
        for(int x = 0; x < 12; x++) {
            for(int y = 7; y < 8; y++) {
                addObstacle(x, y);
            }
        }
        // Left side
        for(int x = 0; x < 1; x++) {
            for(int y = 0; y < 7; y++) {
                addObstacle(x, y);
            }
        }
        // Right side
        for(int x = 11; x < 12; x++) {
            for(int y = 0; y < 7; y++) {
                addObstacle(x, y);
            }
        }

        setNode(1, 2, false, "A", Node::Type::ROOT);
        setNode(6, 2, false, "B", Node::Type::ROOT);
        setNode(5, 4, false, "C", Node::Type::ROOT);
        setNode(7, 3, true, "ABC", Node::Type::END);
        drawGate(1, 5, Gate::Operator::OR, Gate::Direction::SOUTH);
        drawGate(5, 3, Gate::Operator::NOT, Gate::Direction::EAST);
        drawGate(7, 6, Gate::Operator::OR, Gate::Direction::EAST);
        drawGate(10, 3, Gate::Operator::NOT, Gate::Direction::NORTH);

        break;

    case 5:
        setNode(0, 0, false, "A", Node::Type::ROOT);
        setNode(0, 3, true, "D", Node::Type::ROOT);
        setNode(3, 1, true, "D", Node::Type::ROOT);
        setNode(4, 1, false, "C", Node::Type::ROOT);
        setNode(1, 7, true, "B", Node::Type::ROOT);
        drawGate(1, 2, Gate::Operator::NOT, Gate::Direction::EAST);
        drawGate(1, 5, Gate::Operator::OR, Gate::Direction::SOUTH);
        addObstacle(3,5);
        addObstacle(3,6);
        drawGate(6, 1, Gate::Operator::OR, Gate::Direction::EAST);
        drawGate(5, 4, Gate::Operator::AND, Gate::Direction::EAST);
        setNode(6, 3, true, "B", Node::Type::ROOT);
        drawGate(5, 7, Gate::Operator::AND, Gate::Direction::EAST);
        drawGate(8, 7, Gate::Operator::NOT, Gate::Direction::EAST);
        setNode(9, 1, false, "C", Node::Type::ROOT);
        drawGate(8, 3, Gate::Operator::AND, Gate::Direction::SOUTH);
        drawGate(9, 4, Gate::Operator::NOT, Gate::Direction::EAST);
        drawGate(10, 6, Gate::Operator::AND, Gate::Direction::SOUTH);
        setNode(11, 7, true, "ABCD", Node::Type::END);

        addObstacle(6,6);
        addObstacle(7,6);
        addObstacle(7,5);
        addObstacle(8,5);
        addObstacle(7,4);
        addObstacle(7,3);
        addObstacle(7,2);
        addObstacle(10,1);

        break;

    default:
        setNode(0, 3, true, "A", Node::Type::ROOT);
        setNode(0, 5, false, "B", Node::Type::ROOT);
        setNode(8, 3, true, "AB", Node::Type::END);
        addObstacle(0, 0);

        drawGate(6, 4, Gate::Operator::AND, Gate::Direction::EAST);
        drawGate(7, 1, Gate::Operator::OR, Gate::Direction::SOUTH);
        drawGate(8, 7, Gate::Operator::NOT, Gate::Direction::EAST);

        break;
    }

    // Reset victory state after setting up new level
    isVictory = false;
}

void Level::removeConfetti() {
    confetti->clearConfetti();
    isVictory = false;
}

// Did not use this for Clear Button. But could use this for moving to the next level.
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

void Level::addDoubleGate(int x, int y, Gate::Operator gateType, Gate::Direction dir) {
    // Later, these will depend on a direction (horizontal or vertical) passed into the method.
    int xOffset = 0;
    int yOffset = 0; // Below this Y coordinate.

    calculateGateOffset(dir, xOffset, yOffset);

    // Check Location for the second gate
    if (x + xOffset < 0 || x + xOffset >= WIDTH || y + yOffset < 0 || y + yOffset >= HEIGHT )
        return;

    if (getGate(x + xOffset, y + yOffset) || getWire(x + xOffset, y + yOffset) ||
        getNode(x + xOffset, y + yOffset) || getObstacle(x + xOffset, y + yOffset))
        return;

    Gate::Ports firstAlignment = Gate::Ports::INOUT;
    Gate::Ports secondAlignment = Gate::Ports::IN;

    // Create related gate objects.
    Gate* firstHalf = new Gate(x, y, gateType, firstAlignment, dir, this);
    Gate* secondHalf = new Gate(x + xOffset, y + yOffset, gateType, secondAlignment, dir, this);

    // Specify relation between
    firstHalf->setOtherHalf(secondHalf);
    secondHalf->setOtherHalf(firstHalf);

    // Draw a gate at the given x, y position
    gateGrid[y * WIDTH + x] = firstHalf;

    // Draw a gate adjacent to firstHalf in the specified direction.
    gateGrid[(y + yOffset) * WIDTH + (x + xOffset)] = secondHalf;
}

void Level::addSingleGate(int x, int y, Gate::Operator gateType, Gate::Direction dir) {
    Gate::Ports alignment = Gate::Ports::INOUT;

    // Create related gate objects.
    Gate* newNotGate = new Gate(x, y, gateType, alignment, dir, this);

    // Draw a gate at the given x, y position
    gateGrid[y * WIDTH + x] = newNotGate;
    qDebug() << "single gate added: " << gateGrid[y * WIDTH + x];
}

void Level::calculateGateOffset(Gate::Direction dir, int& xOffset, int& yOffset) {
    switch(dir) {
    case Gate::Direction::NORTH:
        xOffset = -1;
        yOffset = 0;
        break;
    case Gate::Direction::EAST:
        xOffset = 0;
        yOffset = -1;
        break;
    case Gate::Direction::SOUTH:
        xOffset = 1;
        yOffset = 0;
        break;
    case Gate::Direction::WEST:
        xOffset = 0;
        yOffset = 1;
        break;
    default:
        xOffset = 0;
        yOffset = 0;
        break;
    }
}

void Level::addObstacle(int x, int y) {
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if(obstacleGrid[y * WIDTH + x] == nullptr) {
            obstacleGrid[y * WIDTH + x] = new Obstacle(this);
        }
    }
}

void Level::removeTails(Node* startingNode) {
    Wire* currentWire = startingNode->getWire()->getTailConnection();
    while (currentWire) {
        Wire* tailWire = currentWire->getTailConnection();
        int x = currentWire->getX();
        int y = currentWire->getY();
        if (getNode(x, y))
            return;
        else if (getGate(x, y)) {
            Gate* checkGate = getGate(x, y);
            qDebug() << "found gate: " << checkGate;

            // Gets the valid output wire:
            if (checkGate->getAlignment() == Gate::Ports::INOUT)
                currentWire = checkGate->getOutputNode()->getWire();
            else
                currentWire = checkGate->getOtherHalf()->getOutputNode()->getWire();

            // Checks to see if the gate wire continues. If so, continue the loop.
            currentWire = currentWire->getTailConnection();
            if (currentWire == nullptr)
                return;
            x = currentWire->getX();
            y = currentWire->getY();
            tailWire = currentWire->getTailConnection();
        }
        setWire(x, y, nullptr);
        currentWire = tailWire;
    }
}

void Level::removeTails(Gate* startingGate) {

    if (startingGate->getOutputNode() == nullptr)
        return;

    Wire* currentWire = startingGate->getOutputNode()->getWire()->getTailConnection();
    while (currentWire) {
        Wire* tailWire = currentWire->getTailConnection();
        int x = currentWire->getX();
        int y = currentWire->getY();
        if (getNode(x, y))
            return;
        else if (getGate(x, y)) {
            Gate* checkGate = getGate(x, y);
            qDebug() << "found gate: " << checkGate;

            // Gets the valid output wire:
            if (checkGate->getAlignment() == Gate::Ports::INOUT)
                currentWire = checkGate->getOutputNode()->getWire();
            else
                currentWire = checkGate->getOtherHalf()->getOutputNode()->getWire();

            // Checks to see if the gate wire continues. If so, continue the loop.
            currentWire = currentWire->getTailConnection();
            if (currentWire == nullptr)
                return;
            x = currentWire->getX();
            y = currentWire->getY();
            tailWire = currentWire->getTailConnection();
        }
        setWire(x, y, nullptr);
        currentWire = tailWire;
    }
}

bool Level::isEmptySpace(int x, int y) {
    return (getWire(x, y) == nullptr) && (getNode(x, y) == nullptr) &&
        (getObstacle(x, y) == nullptr) && (getGate(x, y) == nullptr);
}


    
void Level::drawGate(int x, int y, Gate::Operator op, Gate::Direction dir) {

    if (x < 0 || x >= WIDTH  || y < 0 || y >= HEIGHT )
        return;


    // Ensure there is nothing already in this grid square.
    if (getGate(x, y) || getWire(x, y) || getNode(x, y) || getObstacle(x, y))
        return;

    // Add the gate to the backend.
    if(op == Gate::Operator::NOT) {
        addSingleGate(x, y, op, dir);
    }
    else {
        addDoubleGate(x, y, op, dir);
    }
}

void Level::clearWires() {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        Wire* wire = wireGrid[i];

        bool isNodeWire = false;
        for (int j = 0; j < WIDTH * HEIGHT; j++) {
            if (nodeGrid[j] && nodeGrid[j]->getWire() == wire) {
                isNodeWire = true;
                break;
            }
        }

        if (!isNodeWire && wire) {
            delete wire;
        }

        wireGrid[i] = nullptr;
    }

    // Reset connections on backing wires (nodes still exist)
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        if (nodeGrid[i]) {
            Wire* backWire = nodeGrid[i]->getWire();
            if (backWire) {
                // reset only the opposite end
                if (nodeGrid[i]->getNodeType() == Node::Type::ROOT)
                    backWire->setTailConnection(nullptr);
                else if (nodeGrid[i]->getNodeType() == Node::Type::END)
                    backWire->setHeadConnection(nullptr);
            }
        }
    }

    isVictory = false;
}

void Level::clearGates() {
    for(int i = 0; i < WIDTH * HEIGHT; i++) {
        if (gateGrid[i]) {
            Gate* gate = gateGrid[i];
            gate->setSignal(false);

            Node* inputNode = gate->getInputNode();
            if (inputNode) {
                Wire* inputWire = inputNode->getWire();
                if (inputWire && inputNode->getNodeType() == Node::Type::ROOT) {
                    inputWire->setHeadConnection(inputWire);
                    inputWire->setTailConnection(nullptr);
                } else if (inputWire && inputNode->getNodeType() == Node::Type::END) {
                    inputWire->setHeadConnection(nullptr);
                    inputWire->setTailConnection(inputWire);
                }
            }
            Node* outputNode = gate->getOutputNode();
            if (outputNode) {
                Wire* outputWire = outputNode->getWire();
                if (outputWire && outputNode->getNodeType() == Node::Type::ROOT) {
                    outputWire->setHeadConnection(outputWire);
                    outputWire->setTailConnection(nullptr);
                } else if (outputWire && outputNode->getNodeType() == Node::Type::END) {
                    outputWire->setHeadConnection(outputWire);
                    outputWire->setTailConnection(nullptr);
                }
            }
        }
    }
}

void Level::clearNodes() {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        if (nodeGrid[i]) {
            Node* node = nodeGrid[i];
            Wire* backWire = node->getWire();
            if (!backWire)
                continue;

            int bx = backWire->getX();
            int by = backWire->getY();

            if (bx < 0 || bx >= WIDTH || by < 0 || by >= HEIGHT)
                continue;

            if (wireGrid[by * WIDTH + bx] != backWire)
                continue;

            if (node->getNodeType() == Node::Type::ROOT) {
                backWire->setHeadConnection(backWire);
                backWire->setTailConnection(nullptr);
            } else if (node->getNodeType() == Node::Type::END) {
                backWire->setHeadConnection(nullptr);
                backWire->setTailConnection(backWire);
            }

            wireGrid[by * WIDTH + bx] = backWire;
        }
    }
}


