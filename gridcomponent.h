/**
 * Abstract parent class for all of the different.
 *
 * All other methods should be virtual.
 *
 * @authors Max Donaldson, Jason Khong,
 * Ariadne Petroulakis, Evan Tucker, and Ian Weller
 *
 * @version 7 April 2025
 */

#ifndef GRIDCOMPONENT_H
#define GRIDCOMPONENT_H

#include <QObject>

class GridComponent : public QObject {

    Q_OBJECT

public:
    /**
     * @brief Obstacle - creates a GridComponent object
     * @param parent - the parent object.
     */
    explicit GridComponent(QObject *parent = nullptr);

    /**
     * Destructor for the Gate class.
     */
    ~GridComponent();

    enum class Type {WIRE, GATE, NODE, OBSTACLE, EMPTY};

    /**
     * @brief getType - Returns the type of GridComponent object this is as an enum.
     * By default, this is EMPTY.
     * @return A Operator enum.
     */
    virtual GridComponent::Type getType();

signals:

};

#endif // GRIDCOMPONENT_H
