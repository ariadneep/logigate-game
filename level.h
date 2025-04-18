/**
 * This class represents a level. A level should have all the
 * necessary components that change from level to level.
 *
 * @authors Max Donaldson, Jason Khong,
 * Ariadne Petroulakis, Evan Tucker, and Ian Weller
 *
 * @version 7 April 2025
 */

#ifndef LEVEL_H
#define LEVEL_H

#include <QObject>
#include <QGraphicsScene>
#include <Box2D/Box2D.h>
#include "confettieffect.h"

#include "wire.h"
#include "gate.h"
#include "node.h"
#include "obstacle.h"

class Level : public QObject {

    Q_OBJECT

public:
    /**
     * @brief Level - Default constructor.
     * @param levelNum - The level chosen to set up.
     * @param graphicsScene - The graphics scene
     * @param box2DWorld - The Box2D world.
     * @param parent - the parent object.
     */
    explicit Level(int levelNum, QGraphicsScene* graphicsScene, b2World* box2DWorld, QObject *parent);

    /**
     * Destructor for the level class.
     */
    ~Level();

    /**
    * @brief victory - Occurs when victory condition happens.
    */
    void victory();

    /**
    * @brief spawnConfetti - spawns confetti upon victory.
    */
    void spawnConfetti();

    /**
     * @brief drawWire - Places a wire at the grid coordinates depending on the context
     * of the coordinates.
     * @param x - X position on the game grid.
     * @param y - Y position on the game grid.
     */
    void drawWire(int x, int y, QString tag);

    /**
     * @brief WIDTH - The width of the level.
     */
    static const int WIDTH = 12;

    /**
     * @brief HEIGHT - The height of the level.
     */
    static const int HEIGHT = 8;

    /**
     * @brief getWire
     * @param x
     * @param y
     * @return
     */
    Wire* getWire(int x, int y);

    /**
     * @brief setWireTemp TEMP METHOD, REMOVE LATER
     * @param x
     * @param y
     */
    void setWireTemp(int x, int y, QString tag);

    /**
     * @brief getGate
     * @param x
     * @param y
     * @return
     */
    Gate* getGate(int x, int y);

    /**
     * @brief getNode
     * @param x
     * @param y
     * @return
     */
    Node* getNode(int x, int y);

    /**
     * @brief setNode TEMP METHOD, REMOVE LATER
     * @param x
     * @param y
     */
    void setNode(int x, int y, QString tag, Node::Type type);

    /**
     * @brief getObstacle
     * @param x
     * @param y
     * @return
     */
    Obstacle* getObstacle(int x, int y);

    /**
     * @brief nodeConnect
     * @param x
     * @param y
     * @param tag
     * @param currentWire
     */
    void nodeConnect(int x, int y, QString tag, Wire* currentWire);

    /**
     * @brief updateLevel - Updates the level to initial start-up procedure.
     */
    void updateLevel();

    /**
     * @brief removeConfetti - Removes confetti from the level.
     */
    void removeConfetti();

    /**
     * @brief clearLevel - Clears the level.
     */
    void clearLevel();

    /**
     * @brief removeTails
     * @param startingNode
     */
    void removeTails(Node* startingNode);

private:

    /**
     * @brief description - The prefacing paragraph for the level.
     */
    QString description;

    /**
     * @brief box2DWorld - The Box2D World.
     */
    b2World* box2DWorld;

    /**
     * @brief graphicsScene - The scene for handling confetti.
     */
    QGraphicsScene* graphicsScene;

    /**
     * @brief confetti - The confetti instance.
     */
    Confetti* confetti;

    /**
     * @brief isVictory - Determines whether the player has won.
     */
    bool isVictory;

    /**
     * @brief wireGrid
     */
    Wire* wireGrid[WIDTH * HEIGHT];

    /**
     * @brief gateGrid
     */
    Gate* gateGrid[WIDTH * HEIGHT];

    /**
     * @brief nodeGrid
     */
    Node* nodeGrid[WIDTH * HEIGHT];

    /**
     * @brief obstacleGrid
     */
    Obstacle* obstacleGrid[WIDTH * HEIGHT];

    /**
     * @brief wireCheck - Private helper to reduce the redundency in the wireDraw method.
     * If the checks fail, then the code returns with nothing executed. Checks to see
     * if there is a wire at the space, if it has the same tag as the current tag, and
     * if the checked space is connected to another wire already.
     * @param currentWire - The space of the wire to be updated.
     * @param checkingWire - The space of the wire to retrieve context from.
     */
    void wireCheck(Wire* currentWire, Wire* checkingWire);

    /**
     * @brief wireConnect
     * @param x
     * @param y
     * @param tag
     * @param currentWire
     */
    void wireConnect(int x, int y, QString tag, Wire* currentWire);

    /**
     * @brief wireRemove
     * @param currentWire
     */
    void wireRemove(Wire* currentWire);

    /**
     * @brief findWire - Checks all coordinates nearby and returns a pointer to a valid wire
     * nearby. Valid wires are: Same tag, not connected to a tail, not null.
     * The Wire::Direction enum reference is passed through to store the direction at which the
     * wire was found.
     * @param x - The coordinate x position to check around.
     * @param y - The coordinate y position to check around.
     * @param tag - The tag of the wire to link up to.
     * @param wireConnectionDirection - The relative direction of where the wire was found at the
     * coordinates.
     * @return The pointer to the neighboring valid wire.
     */
    Wire* findWire(int x, int y, QString tag, Wire::Direction& wireConnectionDirection);

    /**
     * @brief findNode - Checks all coordinates nearby and returns a pointer to a valid node
     * nearby. Valid nodes are: Same tag, not connected, not null, in a connection direction
     * equal to where the node was found or has a direction of NONE.
     * The Wire::Direction enum reference is passed through to store the direction at which the
     * node was found.
     * @param x - The coordinate x position to check around.
     * @param y - The coordinate y position to check around.
     * @param tag - The tag of the node to link up to.
     * @param wireConnectionDirection - The relative direction of where the node was found at the
     * coordinates.
     * @return The pointer to the neighboring valid node.
     */
    Node* findNode(int x, int y, QString tag, Wire::Direction& wireConnectionDirection);

    /**
     * @brief connectWires
     * @param x
     * @param y
     * @param headWire
     * @param tailWire
     */
    void connectWires(Wire* headWire, Wire* tailWire);

    /**
     * @brief nodeDualDirector
     * @param nodeDirection
     * @param wireDirection
     * @return
     */
    Wire::Direction nodeDualDirector(Wire::Direction nodeDirection, Wire::Direction wireDirection);

    /**
     * @brief wireDualDirector
     * @param connectWire
     * @param wireDirection
     * @return
     */
    Wire::Direction wireDualDirector(Wire* connectWire, Wire::Direction wireDirection);

    /**
     * @brief setWire
     * @param x
     * @param y
     * @param newWire
     */
    void setWire(int x, int y, Wire* newWire);

    /**
     * @brief addGate - Adds a gate at the specified x and y position into the level.
     * @param x - The x position.
     * @param y - The y position.
     * @param gateType - The type of gate.
     */
    void addGate(int x, int y, Operator gateType);

    /**
     * @brief addNode - Adds a node at the specified x and y position into the level.
     * @param x - The x position.
     * @param y - The y position.
     * @param tag - The input node.
     * @param nodeType - Specifies whether it is a ROOT or END node.
     * @param signal - The output value, true or false.
     */
    void addNode(int x, int y, QString& tag, Node::Type nodeType, bool signal);

    /**
     * @brief addObstacle - Adds an obstacle at x and y position into the level.
     * @param x - The x position.
     * @param y - The y position.
     */
    void addObstacle(int x, int y);

    /**
     * @brief levelSetup - Sets up the level based on the passed in level number.
     * @param level - The level number chosen to set up.
     */
    void levelSetup(int level);

signals:
    void update();
};

#endif // LEVEL_H
