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

enum class Type {
    AND, OR, NOT
};

class Gate : public GridComponent {

public:
    /**
     * @brief Gate - Makes a Gate object.
     * @param type - AND, OR, or NOT gate.
     * @param parent - the parent object
     */
    explicit Gate(Type type, QObject *parent = nullptr);

    /**
     * Destructor for the Gate class.
     */
    ~Gate();

    /**
     * @brief getType - returns either Type::AND if this gate
     * represents an AND gate, Type::OR if this gate represents
     * an OR gate, or Type::NOT if this gate represents a NOT gate.
     * @return a gate Type
     */
    Type getType();

    /**
     * @brief convertSignal - sets the output signal after checking the
     * two input pulses and performing a boolean operation on them based
     * on the Type of the gate.
     * @param first - the signal of the wire in the gate's first slot
     * @param second - the signal of the wire in the gate's second input slot.
     */
    void convertSignal(bool first, bool second);

    /**
     * @brief convertSignal
     * @param input
     */
    void convertSignal(bool input);

    /**
     * @brief getSignal - returns the output signal
     * @return
     */
    bool getSignal();

private:
    /**
     * @brief gateType - whether this object represents
     * an AND, OR, or NOT gate.
     */
    Type gateType;

    /**
     * @brief outputSignal - the signal currently output
     * by this logic gate. False when the gate is dead,
     */
    bool outputSignal;

};

#endif // GATE_H
