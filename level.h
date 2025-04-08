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

class Level : public QObject {

    Q_OBJECT

public:
    /**
     * @brief Level - creates an Obstacle object
     * @param parent - the parent object.
     */
    explicit Level(int width, QObject *parent = nullptr);

    /**
     * Destructor for the level class.
     */
    ~Level();

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
     * @brief levelNum - the id associated with this specific level
     */
    const int levelNum;

signals:

};

#endif // LEVEL_H
