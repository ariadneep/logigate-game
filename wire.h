/**
 * This class represents a single wire square.
 * A wire is a grid component.
 *
 * @authors Max Donaldson, Jason Khong,
 * Ariadne Petroulakis, Evan Tucker, and Ian Weller
 *
 * @version 7 April 2025
 */

#ifndef WIRE_H
#define WIRE_H

#include <QObject>
#include "gridcomponent.h"

/**
 * @brief The Direction enum should hold the direction
 * the wire is coming from/is facing.
 *
 * VETICAL represents top to bottom
 * HORIZONTAL represents left to right.
 *
 * NONE represents a single unconnected point.
 *
 * NW, NE, SW, and SE all represent turns in the wire.
 * The first letter represents the vertical half and the
 * second represnts the horizontal half.
 */
enum class Direction {
    NONE, VERTICAL, HORIZONTAL, NW, NE, SW, SE
};

class Wire : public GridComponent {

public:
    /**
     * @brief Wire - creates a wire object.
     * @param parent - the parent object
     */
    explicit Wire(QObject *parent = nullptr);

    /**
     * Destructor for the Wire class.
     */
    ~Wire();

    /**
     * @brief getType - Returns the type of GridComponent object this is as an enum.
     * By default, this is WIRE.
     * @return A Operator enum.
     */
    GridComponent::Type getType() override;

private:
    /**
     * @brief tag - represents the wire ID that this wire is a part of.
     */
    QString tag;

    /**
     * @brief wireSignal - represents the current signal
     * going through this wire.
     */
    bool wireSignal;

    /**
     * @brief isDead - if the wire is not connected to any node
     * via other wires or logic gates, the wire is considered dead.
     */
    bool isDead;

    /**
     * @brief isTail - true or false.
     * true if this is an initial point of a wire.
     * true if this is an elbox vertex of a wire.
     * true if this is not connected to any other wires.
     * false if this is the middle of a straight segment of the wire.
     */
    bool isTail;

    /**
     * @brief isHead - true or false.
     * true if this is an endpoint of a line.
     * true if this is an elbow vertex of a wire.
     * true if this is not connected to any other wires.
     * false if this is the middle of a straight segemnt of the wire.
     */
    bool isHead;

};

#endif // WIRE_H
