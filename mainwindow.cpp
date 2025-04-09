#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);
    // CHANGE
    currentLevel = new Level(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete currentLevel;
    gameBoardX = 0;
    gameBoardY = 0;
    newPosition = true;
}

// MOUSE EVENTS
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    double mouseX = event->position().x();
    double mouseY = event->position().y();

    int oldGameBoardX = gameBoardX;
    int oldGameBoardY = gameBoardY;
    QLabel* gameBoard = ui->gameBoard;

    // If the mouse is moved into bounds -> Update canvas coordinates
    if (isInGameBoard(mouseX, mouseY)) {
        // Normalize
        mouseX = mouseX - gameBoard->pos().x();
        mouseY = mouseY - gameBoard->pos().y();

        // Project into Canvas Coords
        // TODO: add in a divisor of the level's width
        gameBoardX = (int)(mouseX / (gameBoard->width() / currentLevel->WIDTH));
        gameBoardY = (int)(mouseY / (gameBoard->height() / currentLevel->HEIGHT));

        //check if the mouse has already been moved into this space before
        if (gameBoardX != oldGameBoardX || gameBoardY != oldGameBoardY)
            newPosition = true;
    }

    if (newPosition) {
        qDebug() << "Moving! MouseX = " << gameBoardX;
        qDebug() << "Moving! MouseY = " << gameBoardY;
        //tool->useTool(sprite, canvasX, canvasY);
        newPosition = false;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    double mouseX = event->position().x();
    double mouseY = event->position().y();
    int oldGameBoardX = gameBoardX;
    int oldGameBoardY = gameBoardY;
    QLabel* gameBoard = ui->gameBoard;

    if (isInGameBoard(mouseX, mouseY)) {
        mouseX = mouseX - gameBoard->pos().x();
        mouseY = mouseY - gameBoard->pos().y();

        // TODO: add in a divisor of the level's width
        gameBoardX = (int)(mouseX / (gameBoard->width() / currentLevel->WIDTH));
        gameBoardY = (int)(mouseY / (gameBoard->height() / currentLevel->HEIGHT));

        //tool->useTool(sprite, canvasX, canvasY);
        if (gameBoardX != oldGameBoardX || gameBoardY != oldGameBoardY)
            newPosition = true;
        else
            newPosition = false;
    }

    if (newPosition) {
        qDebug() << "MouseX = " << gameBoardX;
        qDebug() << "MouseY = " << gameBoardY;
        //tool->useTool(sprite, canvasX, canvasY);
        newPosition = false;
    }
}

bool MainWindow::isInGameBoard(int x, int y){
    bool isValidX = x > ui->gameBoard->pos().x() && x < ui->gameBoard->width() + ui->gameBoard->pos().x();
    bool isValidY = y > ui->gameBoard->pos().y() && y < ui->gameBoard->height() + ui->gameBoard->pos().y();
    return isValidX && isValidY;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    double mouseX = event->position().x();
    double mouseY = event->position().y();

    //if the mouse is not in the canvas, we don't care.
    if(!isInGameBoard(mouseX, mouseY))
        return;

    qDebug() << "Mouse released; sending signal from MAIN to MODEL to save new version.";
    //emit saveNewVersion(sprite);
}
