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
#include "node.h"

/**
 * @brief The Direction enum should hold the direction
 * the wire is coming from/is facing.
 *
 * VETICAL represents top to bottom
 * EW represents left to right.
 *
 * NONE represents a single unconnected point.
 *
 * NW, NE, SW, and SE all represent turns in the wire.
 * The first letter represents the vertical half and the
 * second represnts the horizontal half.
 */

class Node;

class Wire : public GridComponent {

public:

    enum class Direction {
        NONE, NS, EW, NW, NE, SW, SE, N, S, E, W
    };

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
     * @brief getTag
     * @return
     */
    QString getTag();

    /**
     * @brief setTag
     * @param newTag
     */
    void setTag(QString newTag);

    /**
     * @brief isFullyConnected - Checks to see if the wire is connected to another component
     * or not.
     * @return Boolean representation of the connection verification.
     */
    bool isFullyConnected();

    /**
     * @brief getHeadConnection
     * @return
     */
    Wire* getHeadConnection();

    /**
     * @brief setHeadConnection
     */
    void setHeadConnection(Wire* newConnection);

    /**
     * @brief getTailConnection
     * @return
     */
    Wire* getTailConnection();

    /**
     * @brief setTailConnection
     * @param newConnection
     */
    void setTailConnection(Wire* newConnection);

    /**
     * @brief getDirection
     * @return
     */
    Direction getDirection();

    /**
     * @brief setDirection
     * @param newDirection
     */
    void setDirection(Direction newDirection);

    /**
     * @brief getSignal - gets the signal
     * @return Boolean value of signal
     */
    bool getSignal();

    /**
     * @brief setSignal
     * @param newSignal
     */
    void setSignal(bool newSignal);

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

    /**
     * @brief headConnection
     */
    Wire* headConnection;

    /**
     * @brief headConnection
     */
    Wire* tailConnection;

    /**
     * @brief direction
     */
    Direction direction;

    /**
     * @brief signal - The signal value of a wire (i.e. if the wire has
     * been hit with a NOT block)
     */
    bool signal;

    /**
     * @brief nodeEndConnection - this is a pointer to the end node connection, so
     * it can be easily accessed in the wire class (in case you needed to move the
     * wire and disconnect the node it was connected to)
     */
    Node* nodeEndConnection;
};

#endif // WIRE_H
