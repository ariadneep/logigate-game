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

class Level : public QObject {

    Q_OBJECT

public:
    /**
     * @brief Level - creates an Obstacle object
     * @param parent - the parent object.
     * @param graphicsScene - The graphics scene
     * @param box2DWorld - The Box2D world.
     */
    // explicit Level(int width, QGraphicsScene* graphicsScene, b2World* box2DWorld, QObject *parent = nullptr);
        explicit Level(int width, QObject *parent = nullptr);

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

private:
    /**
     * @brief WIDTH the width of the level.
     */
    const int WIDTH;

    /**
     * @brief Gate - Makes a Gate object.
     * @param parent - the parent object
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

    // /**
    //  * @brief levelNum - the id associated with this specific level
    //  */
    // const int levelNum;

signals:

};

#endif // LEVEL_H
