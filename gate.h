/**
 * This class represents a logic gate. Each gate is either
 * an AND, OR, or NOT gate.
 *
 * @authors Max Donaldson, Jason Khong,
 * Ariadne Petroulakis, Evan Tucker, and Ian Weller
 *
 * @version 7 April 2025
 */

#ifndef GATE_H
#define GATE_H

#include <QObject>
#include "gridcomponent.h"
#include "node.h"

class Gate : public GridComponent {

public:
    /**
    * @brief The Operator enum - Holds the operation type performed
    * by this gate.
    *
    * Represents the different types of logic gates.
    */
    enum class Operator {
        AND, OR, NOT
    };

    /**
     * @brief The Alignment enum - Holds information on whether this gate
     * is a first half or a second half.
     */
    enum class Alignment {
        FIRST, SECOND
    };

    /**
     * @brief The Direction enum - Describes the direction tthe output side of this
     * Gate is facing.
     *
     * A gate and its otherHalf must share a rotation.
     */
    enum class Direction {
        NORTH, EAST, SOUTH, WEST
    };

    /**
     * @brief Gate - Makes a Gate object.
     * @param type - AND, OR, or NOT gate.
     * @param parent - The parent object
     */
    explicit Gate(Operator type, Alignment alignment, Direction direction, QObject *parent = nullptr);

    /**
     * Destructor for the Gate class.
     */
    ~Gate();

    /**
     * @brief getOperator - Returns either Operator::AND if this gate
     * represents an AND gate, Operator::OR if this gate represents
     * an OR gate, or Operator::NOT if this gate represents a NOT gate.
     * @return A gate Type
     */
    Operator getOperator();

    /**
     * @brief getSignal - returns the output signal
     * @return the current signal the inputs merge into.
     */
    bool getSignal();

    /**
     * @brief setSignal - Sets output signal to desired output.
     * @param signal - A bool manually set by the user.
     */
    void setSignal(bool signal);

    /**
     * @brief convertSignal - sets the output signal after checking the
     * two input pulses and performing a boolean operation on them based
     * on the Type of the gate.
     * @param firstSignal - the signal of the wire in the gate's first slot
     * @param secondSignal - the signal of the wire in the gate's second input slot.
     * @param firstID - the ID linked with the first signal.
     * @param secondID - the ID linked with the second signal.
     * @return - the new, concatenated ID of the resulting signal conversion/transformation.
     */
    QString convertSignal(bool firstSignal, bool secondSignal, QString firstID, QString secondID);

    /**
     * @brief convertSignal - sets the output signal to an opertion
     * based on a compatible gate Type when given only one input. Keeps
     * track of the ID of the input passed into it.
     * @param input - the boolean signal sent in by the input wire.
     * @param id - the ID associated with the input signal.
     * @return the unchanged ID of the resulting signal conversion/transformation.
     */
    QString convertSignal(bool input, QString id);

    /**
     * @brief getType - Returns the type of GridComponent object this is as an enum.
     * By default, this is GATE.
     * @return A Operator enum.
     */
    GridComponent::Type getType() override;

    /**
     * @brief setOtherHalf - Sets a pointer to the other half of this gate.
     * This should hold a nullptr value when the gate is of a single-block
     * Type like OR and a Gate* of the same Type when the gate is of a two-block Type,
     * like AND or OR.
     * @param otherGate - A Gate pointer to a Gate of the same Gate::Type
     */
    void setOtherHalf(Gate* otherGate);

    /**
     * @brief getAlignment - Getter method for this Gate's Alignment.
     * @return the alignment of this Gate relative to the center position.
     */
    Alignment getAlignment();

    /**
     * @brief getDirection - Getter method for this Gate's Direction.
     * @return the direction this Gate has been rotated to face.
     */
    Direction getDirection();

    /**
     * @brief Gate::getIsInputConnected
     * @return
     */
    bool getIsInputConnected();

    /**
     * @brief getInputDirection
     * @return
     */
    Node::Direction getInputDirection();

    /**
     * @brief connectWire
     * @param connectWire
     * @param connectionDirection
     */
    void connectWire(Wire* connectWire, Wire::Direction connectionDirection);

private:
    /**
     * @brief gateOperator - whether this object represents
     * an AND, OR, or NOT gate.
     */
    Operator gateOperator;

    /**
     * @brief inputNode - takes in a wire input. Nullptr when no wire is connected
     * on the input end.
     */
    Node* inputNode;

    /**
     * @brief outputNode - gives out a wire output. Nullptr when
     * no wire is connected on the output end.
     */
    Node* outputNode;

    /**
     * @brief outputSignal - the signal currently output
     * by this logic gate. False when the gate is dead,
     */
    bool outputSignal;

    /**
     * @brief otherHalf - the other half of the current gate.
     * Null if the gate is of a 1-block type, like Type::NOT.
     */
    Gate* otherHalf;

    /**
     * @brief alignment - The relative alignment of this Gate with respect to its
     * otherHalf.
     */
    Alignment alignment;

    /**
     * @brief direction - the direction this Gate has been rotated to face.
     */
    Direction direction;

};

#endif // GATE_H
