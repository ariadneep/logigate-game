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
#include "gridcomponent.h"

class Level : public QObject {

    Q_OBJECT


public:
    /**
     * @param parent - the parent object.
     */
    explicit Level(QObject *parent = nullptr);

    /**
     * Destructor for the level class.
     */
    ~Level();

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
     * @brief grid - Stores the grid components in a level. Stored as a 1d array that
     * finds coordinates with (y * WIDTH + x).
     */
    GridComponent* grid;

    GridComponent* getGridComponent(int x, int y);

    // /**
    //  * @brief levelNum - the id associated with this specific level
    //  */
    // const int levelNum;

signals:

};

#endif // LEVEL_H
