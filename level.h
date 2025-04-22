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
     * @param graphicsScene - The graphics scene
     * @param box2DWorld - The Box2D world.
     * @param parent - the parent object.
     */
    explicit Level(QGraphicsScene* graphicsScene, b2World* box2DWorld, QObject *parent);

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
    * @brief setNode - Adds a node at the specified x and y position into the level.
    * @brief setNode - Adds a node at the specified x and y position into the level.
    * @param x - The x position.
    * @param y - The y position.
    * @param signal - The output value, true or false.
    * @param tag - The ID of the node.
    * @param type - Specifies whether it is a ROOT or END node.
    */
    void setNode(int x, int y, bool signal, QString tag, Node::Type type);

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

    /**
     * @brief removeTails
     * @param startingGate
     */
    void removeTails(Gate* startingGate);

    /**
     * @brief drawGate - Draws the Gate at a certain X and Y Coordinate
     * @param x - Given X Coordinate
     * @param y - Given Y Coordinate
     * @param op - Given Operator to use
     * @param rot - The direction the gate faces.
     */
    void drawGate(int x, int y, Gate::Operator op, Gate::Direction dir);

    /**
     * @brief levelSetup - Sets up the level based on the passed in level number.
     * @param level - The level number chosen to set up.
     */
    void levelSetup(int level);

    /**
     * @brief isEmptySpace - Checks whether or not the level has a completely empty space at the
     * x and y position.
     * @param x - The x coordinate of the position to check.
     * @param y - The y coordinate of the position to check.
     * @return Whether or not the space is empty.
     */
    bool isEmptySpace(int x, int y);

    /**
     * @brief clearWires - Method to clear the wires on the given level.
     */
    void clearWires();

    /**
     * @brief clearGates - Method to clear the gate signals on the given level.
     */
    void clearGates();

    /**
     * @brief clearNodes - Method to clear the node signals on the given level.
     */
    void clearNodes();

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
     * Only head-connecting wires are valid.
     * The Wire::Direction enum reference is passed through to store the direction at which the
     * wire was found.
     * @param x - The coordinate x position to check around.
     * @param y - The coordinate y position to check around.
     * @param tag - The tag of the wire to link up to.
     * @param wireConnectionDirection - The relative direction of where the wire was found at the
     * coordinates.
     * @return The pointer to the neighboring valid wire. Returns nullptr otherwise.
     */
    Wire* findWire(int x, int y, QString tag, Wire::Direction& wireConnectionDirection);

    /**
     * @brief findInputNode - Checks all coordinates nearby for a valid END node. Valid nodes are:
     * any END node. This should only be called if the head connection of the current wire has
     * already been established.
     * The Wire::Direction enum reference is passed through to store the direction at which the
     * node was found.
     * @param x - The coordinate x position to check around.
     * @param y - The coordinate y position to check around.
     * @param wireConnectionDirection - The relative direction of where the node was found at the
     * coordinates.
     * @return The pointer to the neighboring valid END node. Returns nullptr otherwise.
     */
    Node* findInputNode(int x, int y, Wire::Direction& wireConnectionDirection);

    /**
     * @brief findOutputNode - Checks all coordinates nearby and returns a pointer to a valid ROOT node.
     * nearby. Valid nodes are: Same tag, not connected, not null, in a connection direction
     * equal to where the node was found or has a direction of NONE.
     * The Wire::Direction enum reference is passed through to store the direction at which the
     * node was found.
     * @param x - The coordinate x position to check around.
     * @param y - The coordinate y position to check around.
     * @param tag - The tag of the node to link up to.
     * @param wireConnectionDirection - The relative direction of where the node was found at the
     * coordinates.
     * @return The pointer to the neighboring valid ROOT node. Returns nullptr otherwise.
     */
    Node* findOutputNode(int x, int y, QString tag, Wire::Direction& wireConnectionDirection);

    /**
     * @brief findInputGate - Checks all coordinates nearby and returns a pointer to a valid
     * gate input. Valid gates are: Not fully connected, matching input direction.
     * The Wire::Direction enum reference is passed through to store the direction at which the
     * node was found.
     * @param x - The coordinate x position to check around.
     * @param y - The coordinate y position to check around.
     * @param wireConnectionDirection - The relative direction of where the node was found at the
     * coordinates.
     * @return The pointer to the neighboring valid input gate. Returns nullptr otherwise.
     */
    Gate* findInputGate(int x, int y, Wire::Direction& wireConnectionDirection);

    /**
     * @brief findOutputGate - Checks all coordinates nearby and returns a pointer to a valid
     * gate output. Valid gates are: Fully connected, matching output direction, matching tag.
     * The Wire::Direction enum reference is passed through to store the direction at which the
     * node was found.
     * @param x - The coordinate x position to check around.
     * @param y - The coordinate y position to check around.
     * @param wireConnectionDirection - The relative direction of where the node was found at the
     * coordinates.
     * @return The pointer to the neighboring valid output gate. Returns nullptr otherwise.
     */
    Gate* findOutputGate(int x, int y, QString tag, Wire::Direction& wireConnectionDirection);

    /**
     * @brief setWire - Updates the wire pointer on the grid and sets the head and tail connections
     * to match.
     * @param x - The x-coordinate of the Wire to change.
     * @param y - The y-coordinate of the Wire to change.
     * @param newWire - The new Wire pointer to set to.
     */
    void setWire(int x, int y, Wire* newWire);

    /**
     * @brief addDoubleGate - Adds a double gate (AND or OR) at the specified x and y position into the level.
     * @param x - The x position.
     * @param y - The y position.
     * @param gateType - The type of gate.
     * @param dir - Where the output side of the gate is facing.
     */
    void addDoubleGate(int x, int y, Gate::Operator gateType, Gate::Direction dir);

    /**
     * @brief addSingleGate - Adds a single gate (NOT) at the specified x and y position
     * @param x - The x position.
     * @param y - The y position.
     * @param gateType - The type of gate.
     * @param dir - Where the output side of the gate is facing.
     */
    void addSingleGate(int x, int y, Gate::Operator gateType, Gate::Direction dir);

    /**
     * @brief addObstacle - Adds an obstacle at x and y position into the level.
     * @param x - The x position.
     * @param y - The y position.
     */
    void addObstacle(int x, int y);

    /**
     * @brief calculateGateOffset - Helper method to calculate the offset of
     * the x and y positions for the otherHalf of this gate.
     * If xOffset is nonzero, yOffset must be 0.
     * If yOffset is nonzerol xOffset must be 0.
     * @param dir - the direction the output side of this Gate is facing.
     * @param xOffset - 0, 1, or -1.
     * @param yOffset- 0, 1, or -1.
     */
    void calculateGateOffset(Gate::Direction dir, int& xOffset, int& yOffset);

    /**
     * @brief placeBorder - Helper method that places a 2-block wide obstacle border in the
     * current level.
     */
    void placeBorder();

signals:

    /**
     * @brief update - Denotes that the state of the level has been updated.
     */
    void update();
};

#endif // LEVEL_H
