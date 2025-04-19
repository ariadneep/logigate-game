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

class Node : public GridComponent {

public:
    enum class Type {
        ROOT, END
    };

    enum class Direction {
        NONE, N, E, S, W
    };

    /**
     * @brief Node - Creates a Node object
     * @param parent - The parent object.
     * @param type - Determines if it is the ROOT or END of the node.
     * @param tag - The input node.
     */
    explicit Node(QObject *parent = nullptr, int x = 0, int y = 0,
                  Node::Type type = Node::Type::ROOT, bool signal = true, QString tag = "A");

    /**
     * @brief setPosition
     * @param x
     * @param y
     */
    void setPosition(int x, int y);

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
    Node::Type getNodeType();

    /**
     * @brief checkSignal - Determines the END value of a node.
     * @param tag - The tag to compare against the input node.
     * @param signal - The input node's value.
     * @return A bool representing if the tags match and it is an end node.
     */
    bool checkSignal(QString& tag, bool signal);

    /**
     * @brief getDirection
     * @return
     */
    Node::Direction getDirection();

    /**
     * @brief setDirection
     * @param newDirection
     */
    void setDirection(Node::Direction newDirection);

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
     * @brief getWire
     * @return
     */
    Wire* getWire();

    /**
     * @brief removeTails
     */
    void removeTails();

    /**
     * @brief connectWire
     * @param connectWire
     * @param nodeConnectionDirection
     */
    void connectWire(Wire* connectWire, Wire::Direction nodeConnectionDirection);

private:

    /**
     * @brief direction
     */
    Node::Direction direction;

    /**
     * @brief tag - The input node type.
     */
    QString tag;

    /**
     * @brief signal - The signal value of a node.
     */
    bool signal;

    /**
     * @brief backingWire
     */
    Wire* backingWire;

    /**
     * @brief nodeType - Determines if it is a ROOT or END node.
     */
    Node::Type nodeType;

    /**
     * @brief sprite - The sprite of the node.
     */
    QGraphicsPixmapItem* sprite;

    bool connected;

    Wire::Direction nodeDualDirector(Wire::Direction nodeConnectionDirection,
                                     Wire::Direction wireConnectionDirection);
};

#endif // NODE_H
