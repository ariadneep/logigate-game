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
    void setNode(int x, int y, QString tag);

    /**
     * @brief getObstacle
     * @param x
     * @param y
     * @return
     */
    Obstacle* getObstacle(int x, int y);

    /**
     * @brief This Checks the surrounding nodes and then applies
     * the needed transformations to the pointer wire
     * @param x
     * @param y
     * @param currentWire
     */
    bool nodeCheck(int x, int y, Wire* currentWire);

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
     * @brief levelSetup - Sets up the level based on the passed in level number.
     * @param level - The level number chosen to set up.
     */
    void levelSetup(int level);

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

    void setWire(int x, int y, Wire* newWire);
  
    /**
     * @brief removeTail
     * @param x
     * @param y
     * @param currentWire
     */
    void removeTail(int x, int y, Wire* currentWire);

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
    void addNode(int x, int y, QString& tag, NodeType nodeType, bool signal);

    /**
     * @brief addObstacle - Adds an obstacle at x and y position into the level.
     * @param x - The x position.
     * @param y - The y position.
     */
    void addObstacle(int x, int y);

signals:
    void update();
};

#endif // LEVEL_H
