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
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "gridcomponent.h"
#include "wire.h"

enum class NodeType {
    ROOT, END
};


class Node : public GridComponent {

public:
    /**
     * @brief Node - Creates a Node object
     * @param tag - The input node.
     * @param nodeType - Determines if it is the ROOT or END of the node.
     * @param graphicsScene - The QGraphicsScene for adding sprites.
     * @param xPos - The x-coordinate of the grid.
     * @param yPos - the y-coordinate of the grid.
     * @param parent - The parent object.
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

    /**
     * @brief setTag
     * @param newTag
     */
    void setTag(QString newTag);

    /**
     * @brief getTag - The node's tag.
     * @return A QString representing the tag.
     */
    QString getTag();

    /**
     * @brief getSignal - The signal value of the node.
     * @return A bool representing the signal.
     */
    bool getSignal();

    /**
     * @brief setSignal - Set's the value of the node's signal.
     * @param signal - A bool representing the signal value.
     */
    void setSignal(bool signal);

    /**
     * @brief getNodeType - The type of the node.
     * @return A Type representing whether the node is a ROOT or END.
     */
    NodeType getNodeType();

    /**
     * @brief checkSignal - Determines the END value of a node.
     * @param tag - The tag to compare against the input node.
     * @param signal - The input node's value.
     * @return A bool representing if the tags match and it is an end node.
     */
    bool checkSignal(QString& tag, bool signal);

    /**
     * @brief getX - The node's x-coordinate in the grid.
     * @return The x-coordinate of the node.
     */
    int getX();

    /**
     * @brief getY - The node's y-coordinate in the grid.
     * @return The y-coordinate of the node.
     */
    int getY();

    /**
     * @brief getConnected - Returns if the node has been connected
     * @return bool of if it's connected
     */
    bool getConnected();

    /**
     * @brief setConnected - sets the node's status of connection
     */
    void setConnected(bool connectionStatus);

private:
    /**
     * @brief tag - The input node type.
     */
    QString tag;

    /**
     * @brief signal - The signal value of a node.
     */
    bool signal;

    /**
     * @brief nodeType - Determines if it is a ROOT or END node.
     */
    NodeType nodeType;

    /**
     * @brief sprite - The sprite of the node.
     */
    QGraphicsPixmapItem* sprite;

    bool connected;
};

#endif // NODE_H
