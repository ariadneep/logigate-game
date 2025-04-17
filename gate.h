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


/**
 * @brief The Operator enum - Enum class to hold the Operators
 */
enum class Operator {
    AND, OR, NOT
};

class Gate : public GridComponent {

public:
    /**
     * @brief Gate - Makes a Gate object.
     * @param type - AND, OR, or NOT gate.
     * @param parent - The parent object
     */
    explicit Gate(Operator type, QObject *parent = nullptr);

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
     * @brief getGateTag - Gets the tag for the gate
     * @return returns the gate
     */
    QString getGateTag();

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

private:
    /**
     * @brief gateOperator - whether this object represents
     * an AND, OR, or NOT gate.
     */
    Operator gateOperator;

    /**
     * @brief outputSignal - the signal currently output
     * by this logic gate. False when the gate is dead,
     */
    bool outputSignal;

    /**
     * @brief tag - represents the gate ID.
     */
    QString tag;

    /**
     * @brief otherHalf - the other half of the current gate.
     * Null if the gate is of a 1-block type, like Type::NOT.
     */
    Gate* otherHalf;

};

#endif // GATE_H
