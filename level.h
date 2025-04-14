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
     * @brief setWire TEMP METHOD, REMOVE LATER
     * @param x
     * @param y
     */
    void setWire(int x, int y, QString tag);

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
     * @brief getObstacle
     * @param x
     * @param y
     * @return
     */
    Obstacle* getObstacle(int x, int y);

    /**
     * @brief updateLevel
     */
    void updateLevel();

    /**
     * @brief removeConfetti
     */
    void removeConfetti();

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
    Gate* gateGrid;

    /**
     * @brief nodeGrid
     */
    Node* nodeGrid;

    /**
     * @brief obstacleGrid
     */
    Obstacle* obstacleGrid;

    /**
     * @brief wireCheck - Private helper to reduce the redundency in the wireDraw method.
     * If the checks fail, then the code returns with nothing executed. Checks to see
     * if there is a wire at the space, if it has the same tag as the current tag, and
     * if the checked space is connected to another wire already.
     * @param currentWire - The space of the wire to be updated.
     * @param checkingWire - The space of the wire to retrieve context from.
     */
    void wireCheck(Wire* currentWire, Wire* checkingWire);

    void removeTail(int x, int y, Wire* currentWire);

    // /**
    //  * @brief levelNum - the id associated with this specific level
    //  */
    // const int levelNum;

signals:
    void update();
};

#endif // LEVEL_H
