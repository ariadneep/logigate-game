#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), box2DWorld(nullptr), box2DBody(nullptr), timer(new QTimer(this)), frameCount(0)
{
    ui->setupUi(this);
    setMouseTracking(true);

    gameBoardX = 0;
    gameBoardY = 0;
    newPosition = true;

    /*
     * SETTING UP BOX2D
     */
    graphicsScene = new QGraphicsScene(this);
    graphicsView = new QGraphicsView(graphicsScene, this);
    graphicsView->setFixedSize(800,600);
    graphicsScene->setSceneRect(-400, -300, 800, 600);
    graphicsView->setStyleSheet("background: transparent");

    b2Vec2 gravity(0.0f, 9.8f);
    box2DWorld = new b2World(gravity);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 4.0f);
    b2Body* groundBody = box2DWorld->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 0.1f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    currentLevel = new Level(800, graphicsScene, box2DWorld, this);

    // Confetti instance
    confetti = new Confetti(graphicsScene, box2DWorld);

    // World timer
    connect(timer, &QTimer::timeout, this, &MainWindow::updateWorld);
    timer->start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete currentLevel;
    delete confetti;
    delete box2DWorld;
    delete graphicsScene;
    delete graphicsView;
}

void MainWindow::updateWorld() {
    box2DWorld->Step(1.0f / 60.0f, 6, 2);

    frameCount++;
    if(frameCount == 100) {
        confetti->spawnConfetti();
    }

    confetti->updateConfetti();

    graphicsScene->update();
    graphicsView->update();
    graphicsView->viewport()->update();
    graphicsView->viewport()->repaint();
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
