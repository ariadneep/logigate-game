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
     * @param parent - the parent object
     */
    explicit Gate(QObject *parent = nullptr);

    /**
     * Destructor for the Gate class.
     */
    ~Gate();

private:
    /**
     * @brief gateType - if it's an and, or, or not gate.
     */
    Type gateType;

};

#endif // GATE_H
