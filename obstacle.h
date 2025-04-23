/**
 * This class represents an empty grid component, meant to
 * serve as a blockade for wire placement.
 *
 * @authors Max Donaldson, Jason Khong,
 * Ariadne Petroulakis, Evan Tucker, and Ian Weller
 *
 * @version 22 April 2025
 */

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QObject>

class Obstacle : public QObject {

public:
    /**
     * @brief Obstacle - Creates an Obstacle object.
     * @param parent - The parent object.
     */
    explicit Obstacle(QObject *parent = nullptr);

    /**
     * Destructor for the Obstacle class.
     */
    ~Obstacle();
};

#endif // OBSTACLE_H
