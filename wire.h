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

class Wire : public QObject {

public:

    enum class Direction {
        NONE, NS, EW, NW, NE, SW, SE, N, S, E, W
    };

    /**
     * @brief Wire - Creates a wire object.
     * @param parent - The parent object for the QObject default constructor.
     */
    explicit Wire(QObject *parent = nullptr);

    /**
     * Destructor for the Wire class.
     */
    ~Wire();

    /**
     * @brief getTag - Returns the tag being transfered within this wire.
     * @return The QString stored in the wire for the tag.
     */
    QString getTag();

    /**
     * @brief setTag - Alters the internal tag to the specified QString.
     * @param newTag - The QString to set the backing tag to.
     */
    void setTag(QString newTag);

    /**
     * @brief isFullyConnected - Checks to see if the wire is connected to another component
     * or not.
     * @return Boolean representation of the connection verification.
     */
    bool isFullyConnected();

    /**
     * @brief getHeadConnection - Returns the Wire pointer that comes before this wire in line.
     * The tail connection of the head is a pointer to this Wire.
     * @return The pointer to this wires head.
     */
    Wire* getHeadConnection();

    /**
     * @brief setHeadConnection - Alters the head connection of this wire and sets it to the
     * specified wire pointer.
     * @param newConnection - The wire pointer to set the head connection to.
     */
    void setHeadConnection(Wire* newConnection);

    /**
     * @brief getTailConnection -Returns the Wire pointer that comes after this wire in line.
     * The head connection of the tail is a pointer to this Wire.
     * @return The pointer to this wires tail.
     */
    Wire* getTailConnection();

    /**
     * @brief setTailConnection - Alters the tail connection of this wire and sets it to the
     * specified wire pointer.
     * @param newConnection - The wire pointer to set the tail connection to.
     */
    void setTailConnection(Wire* newConnection);

    /**
     * @brief getDirection - Returns the directions this wire is connected to. If there is only
     * one connection, i.e the tail connection is null, then there is only one direction pointing
     * to the relative location of the head wire. Two directions denote a midpoint in the wire.
     * A NONE direction will not draw the wire.
     * @return The Wire::Direction to draw with.
     */
    Direction getDirection();

    /**
     * @brief setDirection - Setter method for the direction of the wire.
     * @param newDirection - The new Wire::Direction to set the direction to.
     */
    void setDirection(Direction newDirection);

    /**
     * @brief getX - Getter method for the x-coordinate of the grid position for this wire.
     * @return The x-coordinate of the wire.
     */
    int getX();

    /**
     * @brief getY - Getter method for the y-coordinate of the grid position for this wire.
     * @return The y-coordinate of the wire.
     */
    int getY();

    /**
     * @brief setPosition - Setter method for the x and y coordinates of the wire.
     * @param x - An int for the new x-coordinate of the wire.
     * @param y - An int for the new y-coordinate of the wire.
     */
    void setPosition(int x, int y);

    /**
     * @brief getSignal - Gets the signal transfered by the wire, either true or false.
     * @return The bool value of signal.
     */
    bool getSignal();

    /**
     * @brief setSignal - Setter method for the signal transferred in the wire.
     * @param newSignal - A bool for the new backing signal.
     */
    void setSignal(bool newSignal);

    /**
     * @brief connectTail - Connects the current wire to another wire pointer. The specified wire
     * pointer will be connected as a tail and the current wire will be connected as a head.
     * @param connectWire - The wire pointer to link to.
     * @param connectionDirection - The Wire::Direction directing where the tail is being connected
     * relative to the head wire.
     */
    void connectTail(Wire* connectWire, Wire::Direction connectionDirection = Wire::Direction::NONE);

private:
    /**
     * @brief tag - Represents the wire ID that this wire is a part of.
     */
    QString tag;

    /**
     * @brief x - The x position on the grid.
     */
    int x;

    /**
     * @brief y - The y position on the grid.
     */
    int y;

    /**
     * @brief wireSignal - Represents the current signal going through this wire.
     */
    bool wireSignal;

    /**
     * @brief isDead - If the wire is not connected to any node via other wires or logic gates,
     * the wire is considered dead.
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
     * @brief headConnection - A pointer for the wire of the head.
     */
    Wire* headConnection;

    /**
     * @brief headConnection - A pointer for the wire of the tail.
     */
    Wire* tailConnection;

    /**
     * @brief direction - The Wire::Direction of the connections this wire points to.
     */
    Direction direction;

    /**
     * @brief signal - The signal value of a wire (i.e. if the wire has
     * been hit with a NOT block).
     */
    bool signal;

    /**
     * @brief wireDualDirector - Helper method for setting the direction whenever this wire
     * gains a second connection, i.e, the wire becomes fully connected.
     * @param connectionDirection - The Wire::Direction of the connection. Just pass through the
     * connectionDirection from the connectTails method.
     */
    void wireDualDirector(Wire::Direction connectionDirection);
};

#endif // WIRE_H
