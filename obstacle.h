/**
 * This class represents an empty grid component, meant to
 * serve as a blockade for wire placement.
 *
 * An obstacle is a grid component.
 *
 * @authors Max Donaldson, Jason Khong,
 * Ariadne Petroulakis, Evan Tucker, and Ian Weller
 *
 * @version 7 April 2025
 */

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QObject>
#include "gridcomponent.h"

class Obstacle : public GridComponent {

public:
    /**
     * @brief Obstacle - creates an Obstacle object
     * @param parent - the parent object.
     */
    explicit Obstacle(QObject *parent = nullptr);

    /**
     * Destructor for the Obstacle class.
     */
    ~Obstacle();

    /**
     * @brief getType - Returns the type of GridComponent object this is as an enum.
     * By default, this is OBSTACLE.
     * @return A Operator enum.
     */
    GridComponent::Type getType() override;

};

#endif // OBSTACLE_H
