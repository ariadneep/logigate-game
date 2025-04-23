/**
 * This class controls the UI for the EduGame application, as well
 * as UI-model connections.
 *
 * @authors Max Donaldson, Jason Khong,
 * Ariadne Petroulakis, Evan Tucker, and Ian Weller
 *
 * @version 7 April 2025
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <Box2D/Box2D.h>
#include <QGraphicsScene>
#include <QPushButton>
#include <QLabel>
#include <QPropertyAnimation>

#include "level.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {

    Q_OBJECT

protected:
    /**
     * @brief mouseMoveEvent - updates the canvas coordinates whenever
     * the mouse has moved while hovering over the canvas.
     * @param event - mouse movement.
     */
    void mouseMoveEvent(QMouseEvent *event);

    /**
     * @brief mousePressEvent - draws on the canvas at the given coordinates
     * if the mouse is pressed while on the canvas.
     * @param event - mouse clicks.
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @brief mouseReleaseEvent - triggers version history update
     * when the mouse is released.
     * @param event - mouse released.
     */
    void mouseReleaseEvent(QMouseEvent *event);

public:
    /**
     * @brief MainWindow - creates a main window object.
     * @param parent - the parent object
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * MainWindow destructor.
     */
    ~MainWindow();

    /**
     * @brief changeLevel - Handles the event where the level is changed.
     */
    void changeLevel();

private slots:
    /**
     * @brief updateWorld - Updates the world when events trigger.
     */
    void updateWorld();

    /**
     * @brief moveBackground - Manages the background scrolling.
     */
    void moveBackground();

    /**
     * @brief nextLevelButtonClicked - Event for when nextLevelButton is clicked.
     */
    void nextLevelButtonClicked();

    /**
     * @brief clearLevelButtonClicked - Event for when clearLevelButton is clicked.
     */
    void clearLevelButtonClicked();

    /**
     * @brief levelMenuButtonClicked - Event for when levelMenuButton is clicked.
     */
    void levelMenuButtonClicked();

    /**
     * @brief levelOneButtonClicked - Event for when levelOneButton is clicked.
     */
    void levelOneButtonClicked();

    /**
     * @brief levelTwoButtonClicked - Event for when levelTwoButton is clicked.
     */
    void levelTwoButtonClicked();

    /**
     * @brief levelThreeButtonClicked - Event for when levelThreeButton is clicked.
     */
    void levelThreeButtonClicked();

    /**
     * @brief levelFourButtonClicked - Event for when levelFourButton is clicked.
     */
    void levelFourButtonClicked();

    /**
     * @brief levelFiveButtonClicked - Event for when levelFiveButton is clicked.
     */
    void levelFiveButtonClicked();

    /**
     * @brief lessonCloseButtonClicked - Event for when the closeLessonButton is clicked.
     */
    void lessonCloseButtonClicked();

    /**
     * @brief closeStartScreen - Closes the starting screen.
     */
    void closeStartScreen();


private:
    /**
     * @brief ui - Represents the UI with all the components.
     */
    Ui::MainWindow *ui;

    /**
     * @brief gameBoardX - Stores the x-coordinate on the gameboard of the last placed item.
     */
    int gameBoardX;

    /**
     * @brief gameBoardY - Stores the y-coordinate on the gameboard of the last placed item.
     */
    int gameBoardY;

    /**
     * @brief newPosition - Designates whether or not the position set is different from the last
     * stored gameboard position.
     */
    bool newPosition;

    /**
     * @brief componentLayer -
     */
    QPixmap componentLayer;

    /**
     * @brief backgroundLayer -
     */
    QPixmap backgroundLayer;

    /**
     * @brief scrollAnimation
     */
    QPropertyAnimation* scrollAnimation;

    /**
     * @brief scrollDuration
     */
    const int SCROLL_DURATION = 100000;

    /**
    * @brief FALSE_COLOR - the color used for false inputs.
    */
    const QString FALSE_COLOR = "red";

    /**
    * @brief TRUE_COLOR - the color used for true inputs.
    */
    const QString TRUE_COLOR = "blue";

    /**
     * @brief wirePixmaps - Holds the wire pixmaps with direction-color pairs as keys
     * and image Pixmaps as values.
     */
    QHash<QPair<Wire::Direction, QString>, QPixmap> wirePixmaps;

    /**
     * @brief nodePixmaps - Holds the node pixmaps with color as a key
     * and image Pixmaps as values.
     */
    QHash<QPair<bool, Node::Type>, QPixmap> nodePixmaps;

    /**
     * @brief obstaclePixmaps - Holds the obstacle sprite
     */
    QPixmap obstaclePixmaps;

    /**
     * @brief wirePixmaps - Holds the Gate pixmaps with direction-color pairs as keys
     * and image Pixmaps as values.
     */
    QHash<QPair<Gate::Operator, QPair<Gate::Ports, Gate::Direction>>, QPixmap> gatePixmaps;

    /**
     * @brief loadWirePixmaps - Helper method to load all the pixmaps
     * for the wires.
     */
    void loadWirePixmaps();

    /**
     * @brief loadNodePixmaps - Helper method to load all the pixmaps
     * for the nodes.
     */
    void loadNodePixmaps();

    /**
     * @brief loadGatePixmaps - Hellper method to load all the gates to pixmaps
     */
    void loadGatePixmaps();

    /**
     * @brief repaint - Redraws the whole board based on the current level's data.
     *  Reflects backend updates in the frontend.
     */
    void repaint();

    /**
     * @brief paintWire - paints a Wire with the appropriate attributes at the
     * given x, y position.
     * @param x - the horizontal grid square index to draw the wire on.
     * @param y - the vertical grid square index to draw the wire on.
     * @param direction - the direction of the wire.
     * @param signal - the signal associated with the wire color.
     */
    void paintWire(int x, int y, Wire::Direction direction, bool signal);

    /**
     * @brief paintGate - paints a Gate with the appropriate attributes at the
     * given x, y position.
     * @param x - the horizontal grid square index to draw the gate on.
     * @param y - the vertical grid square index to draw the gate on.
     * @param align - the alignment of the gate.
     */
    void paintGate(int x, int y, Gate::Operator op, Gate::Ports align, Gate::Direction dir);

    /**
     * @brief paintNode - paints a node with the appropriate attributes at the
     * given x, y position.
     * @param x - the horizontal grid square index to draw the node on.
     * @param y - the vertical grid square index to draw the node on.
     * @param signal - the signal associated with the color of the Node
     * @param nodeType - this determines if this node will be a root or end.
     */
    void paintNode(int x, int y, bool signal, Node::Type nodeType);

    /**
     * @brief paintObstacle - paints an obstacle with the appropriate attributes at the
     * given x, y position.
     * @param x - the horizontal grid square index to draw the obstacle on.
     * @param y - the vertical grid square index to draw the obstacle on.
     */
    void paintObstacle(int x, int y);

    /**
     * @brief currentTag - The tag associated with the current wire being drawn.
     */
    QString currentTag;

    /**
     * @brief currentLevel - The level currently being played on.
     */
    Level* currentLevel;

    /**
     * @brief isInGameBoard - Boolean statement to declare whether or not an x-y position
     *  is within the bounds of the game board.
     * @param x - the x position to check.
     * @param y - the y position to check.
     * @return True if the mouse is inside of the QLabel (board game) widget.
     */
    bool isInGameBoard(int x, int y);

    /**
     * @brief graphicsScene - The graphics scene.
     */
    QGraphicsScene* graphicsScene;

    /**
     * @brief graphicsView - The graphics view.
     */
    QGraphicsView* graphicsView;

    /**
     * @brief box2DWorld - The Box2D world.
     */
    b2World* box2DWorld;

    /**
     * @brief box2DBody - The Box2D body.
     */
    b2Body* box2DBody;
    
    /**
     * @brief timer - Manages event triggers and steps.
     */
    QTimer* timer;

    /**
     * @brief frameCount - Manages QTimer step counts.
     */
    int frameCount;

    /**
     * @brief levelNum - The current level.
     */
    int levelNum;

    /**
     * @brief levelMenuBody - Box2D for levelMenu Widget.
     */
    b2Body* levelMenuBody;

    /**
     * @brief isLevelMenuShowing - Bool for sliding levelMenu on or off screen.
     */
    bool isLevelMenuShowing;

    /**
     * @brief lessonBody - Box2D for lessonWidget QWidget.
     */
    b2Body* lessonBody;

    /**
     * @brief isLessonShowing - Bool for sliding lessonWidget on or off screen.
     */
    bool isLessonShowing;

    /**
     * @brief lessonText - The text to be shown on the lessonWidget.
     */
    QString lessonText;

    /**
     * @brief setLessonText - A method for setting lessonText.
     */
    void setLessonText();

    /**
     * @brief lessonAcceleration - Acceleration for lessonWidget movement.
     */
    int lessonAcceleration;

};
#endif // MAINWINDOW_H
