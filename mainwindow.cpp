#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QTimer>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), box2DWorld(nullptr), box2DBody(nullptr), timer(new QTimer(this)), frameCount(0), levelNum(0)
{
    //Adding/setting up the font used.
    QFontDatabase::addApplicationFont(":/Code 7x5.ttf");
    // QFont code7x5("Code 7x5", 20);

    ui->setupUi(this);
    setMouseTracking(true);
    ui->gameBoard->setMouseTracking(true);

    ui->levelSelectMenu->setStyleSheet("background: 3b3e3f");
    ui->startingScreen->move(0, 0);
    ui->levelSelectMenu->move(0, 0);

    isLevelMenuShowing = false;
    isLessonShowing = false;
    lessonText = "";

    gameBoardX = 0;
    gameBoardY = 0;
    newPosition = true;

    // Initialize pixmaps
    loadWirePixmaps();
    loadNodePixmaps();
    loadGatePixmaps();
    
    int boardWidth = ui->gameBoard->width();
    int boardHeight = ui->gameBoard->height();

    componentLayer = QPixmap(boardWidth, boardHeight);
    backgroundLayer = QPixmap(":/sprites/grid-12x8.png");

    componentLayer.fill(Qt::transparent);

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

    b2BodyDef lessonBodyDef;
    lessonBodyDef.type = b2_kinematicBody;
    lessonBodyDef.position.Set(0.0f, -200.0f);
    lessonBody = box2DWorld->CreateBody(&lessonBodyDef);


    currentLevel = new Level(graphicsScene, box2DWorld, this);

    currentTag = "";

    currentLevel->levelSetup(levelNum);

    repaint();

    // Level Selection
    connect(ui->levelMenuButton, &QPushButton::clicked, this, &MainWindow::levelMenuButtonClicked);
    connect(ui->levelOneButton, &QPushButton::clicked, this, &MainWindow::levelOneButtonClicked);
    connect(ui->levelTwoButton, &QPushButton::clicked, this, &MainWindow::levelTwoButtonClicked);
    connect(ui->levelThreeButton, &QPushButton::clicked, this, &MainWindow::levelThreeButtonClicked);
    connect(ui->levelFourButton, &QPushButton::clicked, this, &MainWindow::levelFourButtonClicked);
    connect(ui->levelFiveButton, &QPushButton::clicked, this, &MainWindow::levelFiveButtonClicked);

    // Next lvl & reset
    connect(ui->nextLevelButton, &QPushButton::clicked, this, &MainWindow::nextLevelButtonClicked);
    connect(ui->clearLevelButton, &QPushButton::clicked, this, &MainWindow::clearLevelButtonClicked);

    // Lesson connects
    connect(ui->closeLessonButton, &QPushButton::clicked, this, &MainWindow::lessonCloseButtonClicked);

    // Start menu connects.
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::closeStartScreen);

    // Background mover
    scrollAnimation = new QPropertyAnimation(ui->backgroundScroll, "geometry");
    scrollAnimation->setDuration(SCROLL_DURATION);
    scrollAnimation->setStartValue(QRect(0, -250, 1400, 750));
    scrollAnimation->setEndValue(QRect(-700, 0, 1400, 750));
    QTimer::singleShot(0, this, &MainWindow::moveBackground);

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
    delete scrollAnimation;
}

void MainWindow::moveBackground() {
    scrollAnimation->start();
    QTimer::singleShot(SCROLL_DURATION, this, &MainWindow::moveBackground);
}

void MainWindow::updateWorld() {
    box2DWorld->Step(2.0f / 60.0f, 6, 2);

    b2Vec2 levelMenuPosition = levelMenuBody->GetPosition();
    ui->levelSelectMenu->move(levelMenuPosition.x * 100.0f, ui->levelSelectMenu->y());

    // Stop levelMenu at specific positions.
    if(isLevelMenuShowing && levelMenuPosition.x * 100.0f >= 125.0f) {
        levelMenuBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        levelMenuBody->SetTransform(b2Vec2(125.0f / 100.0f, 0.0f), 0.0f);
        ui->levelSelectMenu->move(125, ui->levelSelectMenu->y());
    }
    else if (!isLevelMenuShowing && levelMenuPosition.x * 100.0f <= -200.0f) {
        levelMenuBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        levelMenuBody->SetTransform(b2Vec2(-200.0f / 100.0f, 0.0f), 0.0f);
        ui->levelSelectMenu->move(-200, ui->levelSelectMenu->y());
    }

    // Acceleration
    if (isLessonShowing) {
        b2Vec2 startingVelocity = lessonBody->GetLinearVelocity();
        float updatedVelocity = startingVelocity.y * 1.075f;
        if (startingVelocity.y == 0.0f) {
            updatedVelocity = 0.015f;
        }
        lessonBody->SetLinearVelocity(b2Vec2(0.0f, updatedVelocity));
    } else if (!isLessonShowing) {
        b2Vec2 startingVelocity = lessonBody->GetLinearVelocity();
        float updatedVelocity = startingVelocity.y * 1.05f;
        if (startingVelocity.y == 0.0f) {
            updatedVelocity = -0.03f;
        }
        lessonBody->SetLinearVelocity(b2Vec2(0.0f, updatedVelocity));
    }

    b2Vec2 lessonPosition = lessonBody->GetPosition();
    ui->lessonWidget->move(ui->lessonWidget->x(), lessonPosition.y * 100.0f);

    // Stop lessonWidget at specific positions.
    if(isLessonShowing && lessonPosition.y * 100.0f >= 60.0f) {
        // Move the levelMenuBody off the screen.
        levelMenuBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        levelMenuBody->SetTransform(b2Vec2(-200.0f / 100.0f, 0.0f), 0.0f);
        ui->levelSelectMenu->move(-200, 90);
        isLevelMenuShowing = false;

        lessonBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        lessonBody->SetTransform(b2Vec2(0.0f, 60.0f / 100.0f), 0.0f);
        ui->lessonWidget->move(90, 60);
    }
    else if (!isLessonShowing && lessonPosition.y * 100.0f <= -500.0f) {
        lessonBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        lessonBody->SetTransform(b2Vec2(0.0f, -500.0f / 100.0f), 0.0f);
        ui->lessonWidget->move(90, -500);
    }

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
    wirePixmaps.insert({Wire::Direction::N, "blue"}, QPixmap(":/sprites/blue_wires/blue_plug_down.png"));
    wirePixmaps.insert({Wire::Direction::E, "blue"}, QPixmap(":/sprites/blue_wires/blue_plug_left.png"));
    wirePixmaps.insert({Wire::Direction::S, "blue"}, QPixmap(":/sprites/blue_wires/blue_plug_up.png"));
    wirePixmaps.insert({Wire::Direction::W, "blue"}, QPixmap(":/sprites/blue_wires/blue_plug_right.png"));

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
}

void MainWindow::loadNodePixmaps() {
    // Red buttons
    nodePixmaps.insert({false, Node::Type::ROOT}, QPixmap(":/sprites/red_wires/red_node_root.png"));
    nodePixmaps.insert({false, Node::Type::END}, QPixmap(":/sprites/red_wires/red_node_end.png"));

    // Blue buttons
    nodePixmaps.insert({true, Node::Type::ROOT}, QPixmap(":/sprites/blue_wires/blue_node_root.png"));
    nodePixmaps.insert({true, Node::Type::END}, QPixmap(":/sprites/blue_wires/blue_node_end.png"));
}

void MainWindow::repaint() {
    qDebug() << "repainting the board";

    //Must remove everything already in the layers.
    componentLayer.fill(Qt::transparent);

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
                paintWire(x, y, currentWire->getDirection(), currentWire->getSignal());
            if(currentGate)
                paintGate(x, y, currentGate->getOperator(), currentGate->getAlignment(), currentGate->getDirection());
            if(currentNode)
                paintNode(x, y, currentNode->getSignal(), currentNode->getNodeType());
            if(currentObstacle)
                paintObstacle(x, y);
        }
    }

}

void MainWindow::paintWire(int x, int y, Wire::Direction direction, bool signal) {
    QString color = FALSE_COLOR;

    // Holds the current wire texture to be drawn.
    QPixmap wirePixmap;

    // Change the color to red if the wire tag is A or blue if the wire tag is B.
    if(signal)
        color = TRUE_COLOR;

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

    // Set up the painter and link to componentLayer.
    QPainter wirePainter(&componentLayer);

    // Draw to the painter.
    wirePainter.drawPixmap(uiX, uiY, boxWidth, boxHeight, wirePixmap);

    // Draw to the UI.
    ui->gameBoard->setPixmap(componentLayer);

}


void MainWindow::loadGatePixmaps() {
    // EAST-facing wires.
    gatePixmaps.insert({Gate::Operator::AND, {Gate::Ports::INOUT, Gate::Direction::EAST}},
                       QPixmap(":/sprites/objects/and_bottom.png"));
    gatePixmaps.insert({Gate::Operator::AND, {Gate::Ports::IN, Gate::Direction::EAST}},
                       QPixmap(":/sprites/objects/and_top.png"));
    gatePixmaps.insert({Gate::Operator::OR, {Gate::Ports::INOUT, Gate::Direction::EAST}},
                       QPixmap(":/sprites/objects/or_bottom.png"));
    gatePixmaps.insert({Gate::Operator::OR, {Gate::Ports::IN, Gate::Direction::EAST}},
                       QPixmap(":/sprites/objects/or_top.png"));
    gatePixmaps.insert({Gate::Operator::NOT, {Gate::Ports::INOUT, Gate::Direction::EAST}},
                       QPixmap(":/sprites/objects/not.png"));

    // SOUTH-facing wires.
    gatePixmaps.insert({Gate::Operator::AND, {Gate::Ports::INOUT, Gate::Direction::SOUTH}},
                       QPixmap(":/sprites/objects/and_bottom.png").transformed(QTransform().rotate(90)));
    gatePixmaps.insert({Gate::Operator::AND, {Gate::Ports::IN, Gate::Direction::SOUTH}},
                       QPixmap(":/sprites/objects/and_top.png").transformed(QTransform().rotate(90)));
    gatePixmaps.insert({Gate::Operator::OR, {Gate::Ports::INOUT, Gate::Direction::SOUTH}},
                       QPixmap(":/sprites/objects/or_bottom.png").transformed(QTransform().rotate(90)));
    gatePixmaps.insert({Gate::Operator::OR, {Gate::Ports::IN, Gate::Direction::SOUTH}},
                       QPixmap(":/sprites/objects/or_top.png").transformed(QTransform().rotate(90)));
    gatePixmaps.insert({Gate::Operator::NOT, {Gate::Ports::INOUT, Gate::Direction::SOUTH}},
                       QPixmap(":/sprites/objects/not.png").transformed(QTransform().rotate(90)));

    // WEST-facing wires.
    gatePixmaps.insert({Gate::Operator::AND, {Gate::Ports::INOUT, Gate::Direction::WEST}},
                       QPixmap(":/sprites/objects/and_bottom.png").transformed(QTransform().rotate(180)));
    gatePixmaps.insert({Gate::Operator::AND, {Gate::Ports::IN, Gate::Direction::WEST}},
                       QPixmap(":/sprites/objects/and_top.png").transformed(QTransform().rotate(180)));
    gatePixmaps.insert({Gate::Operator::OR, {Gate::Ports::INOUT, Gate::Direction::WEST}},
                       QPixmap(":/sprites/objects/or_bottom.png").transformed(QTransform().rotate(180)));
    gatePixmaps.insert({Gate::Operator::OR, {Gate::Ports::IN, Gate::Direction::WEST}},
                       QPixmap(":/sprites/objects/or_top.png").transformed(QTransform().rotate(180)));
    gatePixmaps.insert({Gate::Operator::NOT, {Gate::Ports::INOUT, Gate::Direction::WEST}},
                       QPixmap(":/sprites/objects/not.png").transformed(QTransform().rotate(180)));

    // NORTH-facing wires.
    gatePixmaps.insert({Gate::Operator::AND, {Gate::Ports::INOUT, Gate::Direction::NORTH}},
                       QPixmap(":/sprites/objects/and_bottom.png").transformed(QTransform().rotate(270)));
    gatePixmaps.insert({Gate::Operator::AND, {Gate::Ports::IN, Gate::Direction::NORTH}},
                       QPixmap(":/sprites/objects/and_top.png").transformed(QTransform().rotate(270)));
    gatePixmaps.insert({Gate::Operator::OR, {Gate::Ports::INOUT, Gate::Direction::NORTH}},
                       QPixmap(":/sprites/objects/or_bottom.png").transformed(QTransform().rotate(270)));
    gatePixmaps.insert({Gate::Operator::OR, {Gate::Ports::IN, Gate::Direction::NORTH}},
                       QPixmap(":/sprites/objects/or_top.png").transformed(QTransform().rotate(270)));
    gatePixmaps.insert({Gate::Operator::NOT, {Gate::Ports::INOUT, Gate::Direction::NORTH}},
                       QPixmap(":/sprites/objects/not.png").transformed(QTransform().rotate(270)));
}

void MainWindow::paintGate(int x, int y, Gate::Operator op, Gate::Ports ports, Gate::Direction dir) {
    QPixmap gatePixmap;
    // Grab the UI measurements for scaling.
    int boxWidth = ui->gameBoard->width() / currentLevel->WIDTH;
    int boxHeight = ui->gameBoard->height() / currentLevel->HEIGHT;
    int uiX = x * boxWidth;
    int uiY = y * boxHeight;

    // Set the current gate texture, scaled relative to the.
    gatePixmap = gatePixmaps.value({op, {ports, dir}}).scaled(
        boxWidth, boxHeight,
        Qt::KeepAspectRatio,
        Qt::FastTransformation);

    // Set up the painter and link to componentLayer.
    QPainter wirePainter(&componentLayer);

    // Draw to the painter.
    wirePainter.drawPixmap(uiX, uiY, boxWidth, boxHeight, gatePixmap);

    // Draw to the UI.
    ui->gameBoard->setPixmap(componentLayer);
}

void MainWindow::paintNode(int x, int y, bool signal, Node::Type nodeType) {
    // Set default color. This color is retained if the tag is not A or B.
    QString color = FALSE_COLOR;

    // Holds the current wire texture to be drawn.
    QPixmap nodePixmap;

    // Change the color to red if the wire tag is A or blue if the wire tag is B.
    if(signal)
        color = TRUE_COLOR;

    // Grab the UI measurements for scaling.
    int boxWidth = ui->gameBoard->width() / currentLevel->WIDTH;
    int boxHeight = ui->gameBoard->height() / currentLevel->HEIGHT;
    int uiX = x * boxWidth;
    int uiY = y * boxHeight;

    // Set the current wire texture, scaled relative to the.
    nodePixmap = nodePixmaps.value({signal, nodeType}).scaled(
        boxWidth, boxHeight,
        Qt::KeepAspectRatio,
        Qt::FastTransformation);

    // Set up the painter and link to componentLayer.
    QPainter nodePainter(&componentLayer);

    // Draw to the painter.
    nodePainter.drawPixmap(uiX, uiY, boxWidth, boxHeight, nodePixmap);

    // Draw to the UI.
    ui->gameBoard->setPixmap(componentLayer);

}

void MainWindow::paintObstacle(int x, int y) {
    // Grab the UI measurements for scaling.
    int boxWidth = ui->gameBoard->width() / currentLevel->WIDTH;
    int boxHeight = ui->gameBoard->height() / currentLevel->HEIGHT;
    int uiX = x * boxWidth;
    int uiY = y * boxHeight;

    // Holds the current wire texture to be drawn.
    QPixmap obstaclePixmap = QPixmap(":/sprites/objects/obsticle.png").scaled(
        boxWidth, boxHeight,
        Qt::KeepAspectRatio,
        Qt::FastTransformation);

    // Set up the painter and link to componentLayer.
    QPainter obstaclePainter(&componentLayer);

    // Draw to the painter.
    obstaclePainter.drawPixmap(uiX, uiY, boxWidth, boxHeight, obstaclePixmap);

    // Draw to the UI.
    ui->gameBoard->setPixmap(componentLayer);
}

void MainWindow::changeLevel() {
    /**
     * Procedure: clear level, create a new instance of currentLevel, and then
     * set up the level.
     */

    ui->nextLevelButton->setDisabled(true);

    currentLevel->clearLevel();

    delete currentLevel;

    currentLevel = new Level(graphicsScene, box2DWorld, this);

    connect(currentLevel, &Level::levelCompleted, this, [this]() {
        qDebug() << "Level completed signal received!";
        ui->nextLevelButton->setEnabled(true);
    });

    currentLevel->levelSetup(levelNum);


    qDebug() << "Changed to level:" << levelNum;
    repaint();
    currentTag = "";
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

        gameBoardX = (int)(mouseX / (gameBoard->width() / currentLevel->WIDTH));
        gameBoardY = (int)(mouseY / (gameBoard->height() / currentLevel->HEIGHT));

        //tool->useTool(sprite, canvasX, canvasY);
        if (gameBoardX != oldGameBoardX || gameBoardY != oldGameBoardY)
            newPosition = true;
        else
            newPosition = false;
    }

    if (newPosition) {
        if (Node* selectedNode = currentLevel->getNode(gameBoardX, gameBoardY)){
            qDebug() << "Removed tails called.";
            currentTag = selectedNode->getTag();
            currentLevel->removeTails(selectedNode);
        }
        else if (Gate* selectedGate = currentLevel->getGate(gameBoardX, gameBoardY)) {
            qDebug() << "Removed tails called for gate.";
            currentTag = selectedGate->getTag();
            currentLevel->removeTails(selectedGate);
            qDebug() << "Removed tails called for gate is DONE.";
        }
        else
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
        levelMenuBody->SetLinearVelocity(b2Vec2(2.0f, 0.0f));
        isLevelMenuShowing = true;
    } else {
        levelMenuBody->SetLinearVelocity(b2Vec2(-2.0f, 0.0f));
        isLevelMenuShowing = false;
    }
}

void MainWindow::levelOneButtonClicked() {
    /*
     * Procedure:
     * 1. Call the clearLevel method.
     * 2. Call levelLayout(levelNum), based on which button was clicked.
     * Do the same for the other level button clicked public slots.
     */
    levelNum = 1;
    changeLevel();
    setLessonText();
    ui->nextLevelButton->setDisabled(false);

    if(!isLessonShowing) {
        lessonBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        isLessonShowing = true;
        ui->lessonText->setText(lessonText);
    }

    ui->nextLevelButton->setDisabled(true);

    repaint();
}

void MainWindow::levelTwoButtonClicked() {
    levelNum = 2;
    changeLevel();
    setLessonText();
    ui->nextLevelButton->setDisabled(false);

    if(!isLessonShowing) {
        lessonBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        isLessonShowing = true;
        ui->lessonText->setText(lessonText);
    }

    ui->nextLevelButton->setDisabled(true);

    repaint();
}

void MainWindow::levelThreeButtonClicked() {
    levelNum = 3;
    changeLevel();
    setLessonText();
    ui->nextLevelButton->setDisabled(false);

    if(!isLessonShowing) {
        lessonBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        isLessonShowing = true;
        ui->lessonText->setText(lessonText);
    }

    ui->nextLevelButton->setDisabled(true);

    repaint();
}

void MainWindow::levelFourButtonClicked() {
    levelNum = 4;
    changeLevel();
    setLessonText();
    ui->nextLevelButton->setDisabled(false);

    if(!isLessonShowing) {
        lessonBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        isLessonShowing = true;
        ui->lessonText->setText(lessonText);
    }

    ui->nextLevelButton->setDisabled(true);

    repaint();
}

void MainWindow::levelFiveButtonClicked() {
    levelNum = 5;
    changeLevel();
    setLessonText();
    ui->nextLevelButton->setDisabled(true);

    if(!isLessonShowing) {
        lessonBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        isLessonShowing = true;
        ui->lessonText->setText(lessonText);
    }

    ui->nextLevelButton->setDisabled(true);

    repaint();
}

void MainWindow::clearLevelButtonClicked() {
    /*
     * Procedure: Clear the level, and set it up again.
     * or, don't clear it at all, and only reset all wires from nodes A and B?
     */
    // changeLevel();
    currentLevel->clearWires();
    currentLevel->clearGates();
    currentLevel->clearNodes();
    currentLevel->removeConfetti();

    qDebug() << "SETTING LEVEL" << levelNum;

    setLessonText();

    if(!isLessonShowing) {
        lessonBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        isLessonShowing = true;

        ui->lessonText->setText(lessonText);
    }

    repaint();
    qDebug() << "clearing level";
}

void MainWindow::nextLevelButtonClicked() {

    qDebug() << "next level loading";
    /*
     * Procedure: Clear the level, and then set up the next one.
     */
    levelNum++;
    //If already on highest level, loop back to level one.
    if(levelNum >= 5) {
        levelNum = 5;
        ui->nextLevelButton->setDisabled(true);
    }

    setLessonText();

    if(!isLessonShowing) {
        lessonBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        isLessonShowing = true;

        ui->lessonText->setText(lessonText);
    }

    qDebug() << "levelNum: " << levelNum;
    changeLevel();
    repaint();
}


void MainWindow::lessonCloseButtonClicked() {
    if (isLessonShowing) {
        lessonBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        isLessonShowing = false;
    }
}

void MainWindow::setLessonText() {
    /*
     * TODO: Change lessonText to be the description for each level.
     */
    qDebug() << "SETTING LEVEL" << levelNum;

    switch (levelNum) {
    case 1:
        lessonText = "Level 1";
        break;
    case 2:
        lessonText = "Level 2";
        break;
    case 3:
        lessonText = "Level 3";
        break;
    case 4:
        lessonText = "Level 4";
        break;
    case 5:
        lessonText = "Level 5";
        break;
    default:
        lessonText = "Tutorial - Level 0";
        break;
    }
}

void MainWindow::closeStartScreen() {
    ui->startingScreen->hide();
}
