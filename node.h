/**
 * This class represents a block with a state.
 * Specifically, the root nodes (which give the wires
 * a charge) or the end node (which confirms that the
 * final wire's boolean charge is correct.
 *
 * A node is a grid component.
 *
 * @authors Max Donaldson, Jason Khong,
 * Ariadne Petroulakis, Evan Tucker, and Ian Weller
 *
 * @version 7 April 2025
 */


#ifndef NODE_H
#define NODE_H

#include <QObject>
#include "gridcomponent.h"

enum class Type {
    ROOT, END
};

class Node : public GridComponent {

public:
    /**
     * @brief Node - creates a Node object
     * @param parent - the parent object.
     */
    explicit Node(QObject *parent = nullptr);

    /**
     * Destructor for the Node class.
     */
    ~Node();

    /**
     * @brief getType - Returns the type of GridComponent object this is as an enum.
     * By default, this is NODE.
     * @return A Operator enum.
     */
    GridComponent::Type getType() override;

};

#endif // NODE_H
