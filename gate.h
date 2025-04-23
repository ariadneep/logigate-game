/**
 * This class represents a logic gate. Each gate is either
 * an AND, OR, or NOT gate.
 *
 * @authors Max Donaldson, Jason Khong,
 * Ariadne Petroulakis, Evan Tucker, and Ian Weller
 *
 * @version 22 April 2025
 */

#ifndef GATE_H
#define GATE_H

#include <QObject>
#include "node.h"

class Gate : public QObject {

    Q_OBJECT

public:
    /**
    * @brief The Operator enum - Holds the operation type performed
    * by this gate. Represents the different types of logic gates.
    */
    enum class Operator {
        AND, OR, NOT
    };

    /**
     * @brief The Ports enum - Holds information on where wires can connect to this Gate.
     * IN means they can only connect to the input side.
     * INOUT means they can connect to both the input AND the output side.
     */
    enum class Ports {
        IN, INOUT
    };

    /**
     * @brief The Direction enum - Describes the direction the output side of this
     * Gate is facing. A gate and its otherHalf must share a rotation.
     */
    enum class Direction {
        NORTH, EAST, SOUTH, WEST
    };

    /**
     * @brief Gate - Complex constructor that initializes most of the values for the Gate object.
     * @param x - Stores the X-coordinate for the position of this Gate on the game board.
     * @param y - Stores the Y-coordinate for the position of this Gate on the game board.
     * @param type - Stores the type this is. Dependent on the Gate::Operator enum. Either
     * AND, OR, or NOT. AND and OR gates will create a second gate for a second input.
     * @param alignment - Directs whether or not this gate stores an output Node. Dependent on the
     * Gate::Ports enum, either being IN or INOUT. INOUT stores the output Node.
     * @param direction - Orients where the input/output of the Gate will be. Dependent on the
     * Gate::Direction, being NORTH, SOUTH, EAST, or WEST.
     * @param parent - Used by the default QObject constructor to set the parent object.
     */
    explicit Gate(int x, int y, Operator type, Ports alignment, Direction direction,
                  QObject *parent = nullptr);

    /**
     * Destructor for the Gate class.
     */
    ~Gate();

    /**
     * @brief getOperator - Returns either Operator::AND if this gate
     * represents an AND gate, Operator::OR if this gate represents
     * an OR gate, or Operator::NOT if this gate represents a NOT gate.
     * @return The operator type this Gate is.
     */
    Operator getOperator();

    /**
     * @brief getSignal - returns the output signal.
     * @return The current signal the inputs merge into. Dependent on the Gate Operator.
     */
    bool getSignal();

    /**
     * @brief setSignal - Sets output signal to desired output.
     * @param signal - A bool manually set by the user.
     */
    void setSignal(bool signal);

    /**
     * @brief convertSignal - Sets the output signal after checking the
     * two input pulses and performing a boolean operation on them based
     * on the Type of the gate.
     * @param firstSignal - The signal of the wire in the gate's first slot
     * @param secondSignal - The signal of the wire in the gate's second input slot.
     * @param firstID - The ID linked with the first signal.
     * @param secondID - The ID linked with the second signal.
     */
    void convertSignal(bool firstSignal, bool secondSignal, QString firstID, QString secondID);

    /**
     * @brief convertSignal - Sets the output signal to an opertion
     * based on a compatible gate Type when given only one input. Keeps
     * track of the ID of the input passed into it.
     * @param input - The boolean signal sent in by the input wire.
     * @param id - The ID associated with the input signal.
     */
    void convertSignal(bool input, QString id);

    /**
     * @brief getOtherHalf - Getter method to the other Gate object tethered to this. Will return
     * nullptr if no such other half exists.
     * @return A pointer to the other half of the gate.
     */
    Gate* getOtherHalf();

    /**
     * @brief setOtherHalf - Sets a pointer to the other half of this gate.
     * This should hold a nullptr value when the gate is of a single-block
     * Type like OR and a Gate* of the same Type when the gate is of a two-block Type,
     * like AND or OR.
     * @param otherGate - A Gate pointer to a Gate of the same Gate::Type.
     */
    void setOtherHalf(Gate* otherGate);

    /**
     * @brief getAlignment - Getter method for this Gate's Ports.
     * @return The alignment of this Gate relative to the center position.
     */
    Ports getAlignment();

    /**
     * @brief getDirection - Getter method for this Gate's Direction.
     * @return The direction this Gate has been rotated to face.
     */
    Direction getDirection();

    /**
     * @brief getInputNode - Getter for this Gate's inputNode.
     * @return The backing inputNode of this Gate.
     */
    Node* getInputNode();

    /**
     * @brief isFullyConnected - Checks if ALL possible input slots of this Gate are connected.
     * @return True if all input slots connected, false otherwise.
     */
    bool isFullyConnected();

    /**
     * @brief getInputDirection - Getter for the direction necessary to approach the input Node.
     * @return A Node::Direction denoting which direction the input must be approached by.
     */
    Node::Direction getInputDirection();

    /**
     * @brief getOutputNode - Getter for this Gate's outputNode.
     * @return The backing outputNode of this Gate.
     */
    Node* getOutputNode();

    /**
     * @brief getOutputDirection - Getter for the direction necessary to start the Output wire from.
     * @return A Node::Direction denoting which direction the output wire must be drawn from.
     */
    Node::Direction getOutputDirection();

    /**
     * @brief getTag - Getter for the tag in the output.
     * @return The tag current stored in the Gate.
     */
    QString getTag();

    /**
     * @brief connectWire - Connects the gieven wire to the Gate. Either an input or an output
     * depending on the context of the caller. Implements the Node connectWire method.
     * @param connectWire - The pointer to the Wire to link.
     * @param connectionDirection - The direction at which the connection is made.
     */
    void connectWire(Wire* connectWire, Wire::Direction connectionDirection);

private:
    /**
     * @brief x - The X-coordinate for the position the gate is at on the game baord.
     */
    int x;

    /**
     * @brief y - The Y-coordinate for the position the gate is at on the game baord.
     */
    int y;

    /**
     * @brief gateOperator - Whether this object represents an AND, OR, or NOT gate.
     */
    Operator gateOperator;

    /**
     * @brief inputNode - Takes in a wire input. Nullptr when no wire is connected on the
     * input end.
     */
    Node* inputNode;

    /**
     * @brief outputNode - Gives out a wire output. Nullptr when
     * no wire is connected on the output end.
     */
    Node* outputNode;

    /**
     * @brief outputSignal - The signal currently output
     * by this logic gate. False when the gate is dead.
     */
    bool outputSignal;

    /**
     * @brief otherHalf - The other half of the current gate.
     * nullptr if the gate is of a 1-block type, i.e, Type::NOT.
     */
    Gate* otherHalf;

    /**
     * @brief alignment - The relative alignment of this Gate with respect to its
     * otherHalf.
     */
    Ports alignment;

    /**
     * @brief direction - the direction this Gate has been rotated to face.
     */
    Direction direction;

};

#endif // GATE_H
