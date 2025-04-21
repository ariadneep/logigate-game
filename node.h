/**
 * This class represents a block with a state.
 * Specifically, the root nodes (which give the wires
 * a charge) or the end node (which confirms that the
 * final wire's boolean charge is correct.
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
#include "wire.h"

class Node : public QObject {

    Q_OBJECT

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
     * Destructor for the Node class.
     */
    ~Node();

    /**
     * @brief setTag = Sets the tag used for connect wire comparisons.
     * @param newTag - The new QString to set th tag to.
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
     * @brief getDirection - Getter method for the direction the Node requires output/input
     * from. NONE does not limit the node direction.
     * @return The Node::Direction for the direction specifications of the Node.
     */
    Node::Direction getDirection();

    /**
     * @brief setDirection - Setter method of the node direction. Sets the backing direction to
     * the specified value.
     * @param newDirection - The Node::Direction to set to.
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
     * @brief getWire - Getter for the backing wire this Node uses.
     * @return A Pointer for the backing wire.
     */
    Wire* getWire();

    /**
     * @brief connectWire - Attaches the given wire pointer to the backing wire. Altering
     * functionality for different Node types.
     * @param connectWire - The pointer of the wire to connect to. Does not create a new wire.
     * @param nodeConnectionDirection - The direction the connection was made in.
     */
    void connectWire(Wire* connectWire, Wire::Direction nodeConnectionDirection);

private:

    /**
     * @brief direction - Stores the direction the Node requires output/input from. NONE does not
     * limit the node direction.
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
     * @brief backingWire - Stores the wire for the Node. Necessary for managing data storage and
     * wire connections.
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
};

#endif // NODE_H
