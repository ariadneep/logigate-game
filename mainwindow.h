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

private:
    Ui::MainWindow *ui;

    //gameBoard positions
    int gameBoardX;
    int gameBoardY;
    bool newPosition;

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
     * @brief GraphicsScene - The graphics scene.
     */
    QGraphicsScene* GraphicsScene;

    /**
     * @brief box2DWorld - The Box2D world.
     */
    b2World* box2DWorld;

    /**
     * @brief box2DBody - The Box2D body.
     */
    b2Body* box2DBody;


};
#endif // MAINWINDOW_H
