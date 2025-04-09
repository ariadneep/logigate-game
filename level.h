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

#include "gridcomponent.h"

class Level : public QObject {

    Q_OBJECT


public:
    /**
     * @param parent - the parent object.
     * @param graphicsScene - The graphics scene
     * @param box2DWorld - The Box2D world.
     */
    explicit Level(QObject *parent = nullptr);

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
    void drawWire(int x, int y);

    /**
     * @brief WIDTH - The width of the level.
     */
    static const int WIDTH = 12;

    /**
     * @brief HEIGHT - The height of the level.
     */
    static const int HEIGHT = 8;

private:

    /**
     * @brief description - The prefacing paragraph for the level.
     */
    QString description;

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
     * @brief grid - Stores the grid components in a level. Stored as a 1d array that
     * finds coordinates with (y * WIDTH + x).
     */
    GridComponent* grid;
  
    /**
     *@brief Returns the grid component at the specified x, y position. 
     */
    GridComponent* getGridComponent(int x, int y);

    // /**
    //  * @brief levelNum - the id associated with this specific level
    //  */
    // const int levelNum;

signals:

};

#endif // LEVEL_H
