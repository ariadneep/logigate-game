#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), box2DWorld(nullptr), box2DBody(nullptr), timer(new QTimer(this)), frameCount(0), levelNum(0)
{
    ui->setupUi(this);
    setMouseTracking(true);
    ui->gameBoard->setMouseTracking(true);

    ui->levelSelectMenu->setStyleSheet("border: 2px solid grey");
    isLevelMenuShowing = false;

    gameBoardX = 0;
    gameBoardY = 0;
    newPosition = true;

    // Initialize pixmaps
    loadWirePixmaps();
    loadNodePixmaps();
    loadGatePixmaps();
    
    int boardWidth = ui->gameBoard->width();
    int boardHeight = ui->gameBoard->height();

    wireLayer = QPixmap(boardWidth, boardHeight);
    gateLayer = QPixmap(boardWidth, boardHeight);
    nodeLayer = QPixmap(boardWidth, boardHeight);
    obstacleLayer = QPixmap(boardWidth, boardHeight);
    backgroundLayer = QPixmap(":/sprites/grid-12x8.png");

    wireLayer.fill(Qt::transparent);
    gateLayer.fill(Qt::transparent);
    nodeLayer.fill(Qt::transparent);
    obstacleLayer.fill(Qt::transparent);


    // SETTING UP BOX2D
    graphicsScene = new QGraphicsScene(this);
    graphicsView = new QGraphicsView(graphicsScene, this);
    graphicsView->setFixedSize(800,600);
    graphicsScene->setSceneRect(-400, -300, 800, 600);
    graphicsView->setStyleSheet("background: transparent");
    graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents);


    b2Vec2 gravity(0.0f, 9.8f);
    box2DWorld = new b2World(gravity);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 5.6f);
    b2Body* groundBody = box2DWorld->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(9.5f, 0.1f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    b2BodyDef levelMenuBodyDef;
    levelMenuBodyDef.type = b2_kinematicBody;
    levelMenuBodyDef.position.Set(-200.0f, 0.0f);
    levelMenuBody = box2DWorld->CreateBody(&levelMenuBodyDef);

    currentLevel = new Level(levelNum, graphicsScene, box2DWorld, this);

    // \/ CHANGE \/
    currentTag = "A";
    // currentLevel->setWireTemp(0, 0, currentTag);
    currentLevel->setNode(0, 3, currentTag, Node::Type::ROOT);

    repaint();

    // Level Selection
    connect(ui->levelMenuButton, &QPushButton::clicked, this, &MainWindow::levelMenuButtonClicked);
    connect(ui->levelOneButton, &QPushButton::clicked, this, &MainWindow::levelOneButtonClicked);
    connect(ui->levelTwoButton, &QPushButton::clicked, this, &MainWindow::levelTwoButtonClicked);
    connect(ui->levelThreeButton, &QPushButton::clicked, this, &MainWindow::levelThreeButtonClicked);
    connect(ui->levelFourButton, &QPushButton::clicked, this, &MainWindow::levelFourButtonClicked);
    connect(ui->levelFiveButton, &QPushButton::clicked, this, &MainWindow::levelFiveButtonClicked);

    // World timer
    connect(timer, &QTimer::timeout, this, &MainWindow::updateWorld);
    timer->start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete currentLevel;
    delete box2DWorld;
    delete graphicsScene;
    delete graphicsView;
}

void MainWindow::updateWorld() {
    box2DWorld->Step(1.0f / 60.0f, 6, 2);

    frameCount++;
    if(frameCount == 100) {
        currentLevel->victory();
    }

    /*
    * CHANGE THIS IN THE FUTURE FOR WHEN LEVEL CHANGES.
    */
    if(frameCount == 500) {
        currentLevel->removeConfetti();
    }

    b2Vec2 levelMenuPosition = levelMenuBody->GetPosition();
    ui->levelSelectMenu->move(levelMenuPosition.x * 100.0f, ui->levelSelectMenu->y());
    currentLevel->updateLevel();

    graphicsScene->update();
    graphicsView->update();
    graphicsView->viewport()->update();
    graphicsView->viewport()->repaint();
}

void MainWindow::loadWirePixmaps() {
    //bue wires
    wirePixmaps.insert({Wire::Direction::EW, "blue"}, QPixmap(":/sprites/blue_wires/blue_wire_EW.png"));
    wirePixmaps.insert({Wire::Direction::NE, "blue"}, QPixmap(":/sprites/blue_wires/blue_wire_NE.png"));
    wirePixmaps.insert({Wire::Direction::NS, "blue"}, QPixmap(":/sprites/blue_wires/blue_wire_NS.png"));
    wirePixmaps.insert({Wire::Direction::NW, "blue"}, QPixmap(":/sprites/blue_wires/blue_wire_NW.png"));
    wirePixmaps.insert({Wire::Direction::SE, "blue"}, QPixmap(":/sprites/blue_wires/blue_wire_SE.png"));
    wirePixmaps.insert({Wire::Direction::SW, "blue"}, QPixmap(":/sprites/blue_wires/blue_wire_SW.png"));

    //red wires
    wirePixmaps.insert({Wire::Direction::EW, "red"}, QPixmap(":/sprites/red_wires/red_wire_EW.png"));
    wirePixmaps.insert({Wire::Direction::NE, "red"}, QPixmap(":/sprites/red_wires/red_wire_NE.png"));
    wirePixmaps.insert({Wire::Direction::NS, "red"}, QPixmap(":/sprites/red_wires/red_wire_NS.png"));
    wirePixmaps.insert({Wire::Direction::NW, "red"}, QPixmap(":/sprites/red_wires/red_wire_NW.png"));
    wirePixmaps.insert({Wire::Direction::SE, "red"}, QPixmap(":/sprites/red_wires/red_wire_SE.png"));
    wirePixmaps.insert({Wire::Direction::SW, "red"}, QPixmap(":/sprites/red_wires/red_wire_SW.png"));
    wirePixmaps.insert({Wire::Direction::N, "red"}, QPixmap(":/sprites/red_wires/red_plug_down.png"));
    wirePixmaps.insert({Wire::Direction::E, "red"}, QPixmap(":/sprites/red_wires/red_plug_left.png"));
    wirePixmaps.insert({Wire::Direction::S, "red"}, QPixmap(":/sprites/red_wires/red_plug_up.png"));
    wirePixmaps.insert({Wire::Direction::W, "red"}, QPixmap(":/sprites/red_wires/red_plug_right.png"));

    //green wires
    wirePixmaps.insert({Wire::Direction::EW, "green"}, QPixmap(":/sprites/green_wires/green_wire_EW.png"));
    wirePixmaps.insert({Wire::Direction::NE, "green"}, QPixmap(":/sprites/green_wires/green_wire_NE.png"));
    wirePixmaps.insert({Wire::Direction::NS, "green"}, QPixmap(":/sprites/green_wires/green_wire_NS.png"));
    wirePixmaps.insert({Wire::Direction::NW, "green"}, QPixmap(":/sprites/green_wires/green_wire_NW.png"));
    wirePixmaps.insert({Wire::Direction::SE, "green"}, QPixmap(":/sprites/green_wires/green_wire_SE.png"));
    wirePixmaps.insert({Wire::Direction::SW, "green"}, QPixmap(":/sprites/green_wires/green_wire_SW.png"));

}

void MainWindow::loadNodePixmaps() {
    // Red buttons
    nodePixmaps.insert("red", QPixmap(":/sprites/red_wires/red_button_down.png"));

    // Blue buttons
    nodePixmaps.insert("blue", QPixmap(":/sprites/blue_wires/blue_button_down.png"));

    // Green buttons
    nodePixmaps.insert("green", QPixmap(":/sprites/green_wires/green_button.png"));
}

void MainWindow::repaint() {
    qDebug() << "repainting the board";

    //Must remove everything already in the layers.
    wireLayer.fill(Qt::transparent);
    nodeLayer.fill(Qt::transparent);

    //Pointers to hold values of the different grid objects.
    Wire* currentWire;
    Gate* currentGate;
    Node* currentNode;
    Obstacle* currentObstacle;

    //checks for components at each box on the board.
    for(int x = 0; x < currentLevel->WIDTH; x++) {
        for( int y = 0; y < currentLevel->HEIGHT; y++) {
            currentWire = currentLevel->getWire(x, y);
            currentGate = currentLevel->getGate(x, y);
            currentNode = currentLevel->getNode(x, y);
            currentObstacle = currentLevel->getObstacle(x, y);

            if(currentWire)
                paintWire(x, y, currentWire->getDirection(), currentWire->getTag());
            if(currentGate)
                paintGate(x, y, currentGate->getOperator());
            if(currentNode)
                paintNode(x, y, currentNode->getTag());
            if(currentObstacle)
                paintObstacle(x, y);
        }
    }

}

void MainWindow::paintWire(int x, int y, Wire::Direction direction, QString tag) {
    // Set default color. This color is retained if the tag is not A or B.
    QString color = "green";

    // Holds the current wire texture to be drawn.
    QPixmap wirePixmap;

    // Change the color to red if the wire tag is A or blue if the wire tag is B.
    if(!tag.isNull() && tag.toUpper() == "A")
        color = "red";
    else if(!tag.isNull() && tag.toUpper() == "B")
        color = "blue";

    // Grab the UI measurements for scaling.
    int boxWidth = ui->gameBoard->width() / currentLevel->WIDTH;
    int boxHeight = ui->gameBoard->height() / currentLevel->HEIGHT;
    int uiX = x * boxWidth;
    int uiY = y * boxHeight;

    // Set the current wire texture, scaled relative to the.
    wirePixmap = wirePixmaps.value({direction, color}).scaled(
        boxWidth, boxHeight,
        Qt::KeepAspectRatio,
        Qt::FastTransformation);

    // Set up the painter and link to wireLayer.
    QPainter wirePainter(&wireLayer);

    // Draw to the painter.
    wirePainter.drawPixmap(uiX, uiY, boxWidth, boxHeight, wirePixmap);

    // Draw to the UI.
    ui->gameBoard->setPixmap(wireLayer);

}


void MainWindow::loadGatePixmaps() {
    //greed AND gates
    gatePixmaps.insert({Operator::AND, "green"}, QPixmap(":/sprites/green_wires/and_bottom_blue.png"));
    gatePixmaps.insert({Operator::AND, "green"}, QPixmap(":/sprites/green_wires/and_bottom_noconnection.png"));
    gatePixmaps.insert({Operator::AND, "green"}, QPixmap(":/sprites/green_wires/and_top_noconnection.png"));
    gatePixmaps.insert({Operator::AND, "green"}, QPixmap(":/sprites/green_wires/and_top_red.png"));

}

void MainWindow::paintGate(int x, int y, Operator op) {
    // Holds the current gate texture to be drawn.
    QPixmap gatePixmap;
    // Grab the UI measurements for scaling.
    int boxWidth = ui->gameBoard->width() / currentLevel->WIDTH;
    int boxHeight = ui->gameBoard->height() / currentLevel->HEIGHT;
    int uiX = x * boxWidth;
    int uiY = y * boxHeight;

    // Set the current gate texture, scaled relative to the.
    gatePixmap = gatePixmaps.value({op, "AND"}).scaled(
        boxWidth, boxHeight,
        Qt::KeepAspectRatio,
        Qt::FastTransformation);


    // Set up the painter and link to wireLayer.
    QPainter wirePainter(&wireLayer);

    // Draw to the painter.
    wirePainter.drawPixmap(uiX, uiY, boxWidth, boxHeight, gatePixmap);

    // Draw to the UI.
    ui->gameBoard->setPixmap(wireLayer);
}

void MainWindow::paintNode(int x, int y, QString tag) {
    // Set default color. This color is retained if the tag is not A or B.
    QString color = "green";

    // Holds the current wire texture to be drawn.
    QPixmap nodePixmap;

    // Change the color to red if the wire tag is A or blue if the wire tag is B.
    if(!tag.isNull() && tag.toUpper() == "A")
        color = "red";
    else if(!tag.isNull() && tag.toUpper() == "B")
        color = "blue";
    qDebug() << "Node color is " << color << "and its tag is " << tag;

    // Grab the UI measurements for scaling.
    int boxWidth = ui->gameBoard->width() / currentLevel->WIDTH;
    int boxHeight = ui->gameBoard->height() / currentLevel->HEIGHT;
    int uiX = x * boxWidth;
    int uiY = y * boxHeight;

    // Set the current wire texture, scaled relative to the.
    nodePixmap = nodePixmaps.value(color).scaled(
        boxWidth, boxHeight,
        Qt::KeepAspectRatio,
        Qt::FastTransformation);

    qDebug() << "Node pixmap is null???" << nodePixmap.isNull();

    // Set up the painter and link to wireLayer.
    QPainter nodePainter(&wireLayer);

    // Draw to the painter.
    nodePainter.drawPixmap(uiX, uiY, boxWidth, boxHeight, nodePixmap);

    // Draw to the UI.
    ui->gameBoard->setPixmap(wireLayer);

}

void MainWindow::paintObstacle(int x, int y) {

}

void MainWindow::changeLevel() {

    currentLevel->clearLevel();
    delete currentLevel;

    currentLevel = new Level(levelNum, graphicsScene, box2DWorld, this);

    currentLevel->levelSetup(levelNum);

    repaint();
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

    // If this is a new position for the mouse, then attempt to draw a wire.
    if (newPosition) {
        currentLevel->drawWire(gameBoardX, gameBoardY, currentTag);
        newPosition = false;

        //redraw the board.
        repaint();
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
        currentLevel->drawWire(gameBoardX, gameBoardY, currentTag);
        newPosition = false;

        repaint();
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

    qDebug() << "Mouse released";
}

void MainWindow::levelMenuButtonClicked() {
    /*
     * Slide the menu to the right if bool isLevelMenuVisible is false upon clicked.
     * Slide the menu to the left if the bool isLevelMenuVisible is true upon clicked.
     */
    if(!isLevelMenuShowing) {
        levelMenuBody->SetTransform(b2Vec2(125.0f / 100.0f, 0.0f), 0.0f);
        isLevelMenuShowing = true;
    } else {
        levelMenuBody->SetTransform(b2Vec2(-200.0f / 100.0f, 0.0f), 0.0f);
        isLevelMenuShowing = false;
    }
}

void MainWindow::levelOneButtonClicked(){
    /*
     * 1. Call the clearLevel method.
     * 2. Call levelLayout(levelNum), based on which button was clicked.
     * Do the same for the other level button clicked public slots.
     */
    levelNum = 1;
    changeLevel();
}

void MainWindow::levelTwoButtonClicked(){
    levelNum = 2;
    changeLevel();
}

void MainWindow::levelThreeButtonClicked(){
    levelNum = 3;
    changeLevel();
}

void MainWindow::levelFourButtonClicked(){
    levelNum = 4;
    changeLevel();
}

void MainWindow::levelFiveButtonClicked(){
    levelNum = 5;
    changeLevel();
}

