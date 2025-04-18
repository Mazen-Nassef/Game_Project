#include "mainwindow.h"
#include "staticobstacle.h"
#include "movingobstacle.h"
#include "ui_mainwindow.h"
#include "player.h"
#include "uimanager.h"
#include "health.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , uiManager(new UIManager())
    , health(new Health(3))  // Create Health object with 100 max health
{
    ui->setupUi(this);

    scoreLabel = new QLabel(this);
    healthLabel = new QLabel(this);
    timeLabel = new QLabel(this);

    // Set labels with right alignment and make them stand out against any background
    QString styleSheet = "color: black; font: bold 16px; background-color: rgba(255, 255, 255, 150); padding: 2px 5px; border-radius: 3px;";
    scoreLabel->setStyleSheet(styleSheet);
    healthLabel->setStyleSheet(styleSheet);
    timeLabel->setStyleSheet(styleSheet);
    
    // Align text to the right within each label
    scoreLabel->setAlignment(Qt::AlignRight);
    healthLabel->setAlignment(Qt::AlignRight);
    timeLabel->setAlignment(Qt::AlignRight);
    
    // Set initial position (will be updated in updateCamera)
    scoreLabel->move(10, 10);
    healthLabel->move(10, 40);
    timeLabel->move(10, 70);

    scoreLabel->raise();
    healthLabel->raise();
    timeLabel->raise();

    scoreLabel->show();
    healthLabel->show();
    timeLabel->show();

    uiManager->setScoreLabel(scoreLabel);
    uiManager->setHealthLabel(healthLabel);
    uiManager->setTimeLabel(timeLabel);

    // Set default score and health
    score = 0;
    time = 0.0;

    // Set the initial values in UIManager
    uiManager->updateScore(score);
    uiManager->updateHealth(health->get());  // Update health from Health object

    // Create the scene
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 2000, 600); // Extended scene width for scrolling

    // Add platforms to the scene
    // Main floor platform (solid)
    Platform* floor = new Platform(0, 500, 2000, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(floor);
    
    // Add some floating platforms (passthrough)
    Platform* platform1 = new Platform(200, 400, 200, 20);
    Platform* platform2 = new Platform(500, 350, 200, 20);
    Platform* platform3 = new Platform(800, 300, 200, 20);
    Platform* platform4 = new Platform(1100, 250, 200, 20);
    Platform* platform5 = new Platform(1400, 300, 200, 20);
    
    scene->addItem(platform1);
    scene->addItem(platform2);
    scene->addItem(platform3);
    scene->addItem(platform4);
    scene->addItem(platform5);
    
    // Create the player object and store reference
    player = new Player();
    player->setPos(100, 400); // Start the player above the floor
    scene->addItem(player);

    // Connect player's health changes to the UI update
    connect(player, &Player::healthChanged, this, &MainWindow::updatePlayerHealth);
    
    // Set the initial health display
    uiManager->updateHealth(player->getHealth());
    
    // Create the view and set the scene
    view = ui->graphicsView;
    view->setScene(scene);

    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::SmoothPixmapTransform);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // Create two purple obstacles with damage=1 (not bouncy) that will be deleted when stood on
    StaticObstacle* purpleObstacle1 = new StaticObstacle(50, 50, 1);
    purpleObstacle1->setIsBouncy(false);
    purpleObstacle1->setBounceStrength(0.0f);
    purpleObstacle1->setPlayerFriction(1.0f);
    purpleObstacle1->setBrush(QBrush(Qt::magenta)); // Purple color
    purpleObstacle1->setPos(300, 350); // x, y coordinates in the scene
    scene->addItem(purpleObstacle1);
    
    StaticObstacle* purpleObstacle2 = new StaticObstacle(100, 50, 1);
    purpleObstacle2->setIsBouncy(false);
    purpleObstacle2->setBounceStrength(0.0f);
    purpleObstacle2->setPlayerFriction(1.0f);
    purpleObstacle2->setBrush(QBrush(Qt::magenta)); // Purple color
    purpleObstacle2->setPos(700, 400); // x, y coordinates in the scene
    scene->addItem(purpleObstacle2);
    
    // Create two blue obstacles with damage=0 (very bouncy)
    StaticObstacle* blueObstacle1 = new StaticObstacle(100, 50, 0); // No damage
    blueObstacle1->setIsBouncy(true);
    blueObstacle1->setBounceStrength(1.2f); // Very bouncy
    blueObstacle1->setPlayerFriction(0.8f);
    blueObstacle1->setBrush(QBrush(Qt::blue)); // Blue color
    blueObstacle1->setPos(500, 450); // x, y coordinates in the scene
    scene->addItem(blueObstacle1);
    
    StaticObstacle* blueObstacle2 = new StaticObstacle(100, 50, 0); // No damage
    blueObstacle2->setIsBouncy(true);
    blueObstacle2->setBounceStrength(1.2f); // Very bouncy
    blueObstacle2->setPlayerFriction(0.8f);
    blueObstacle2->setBrush(QBrush(Qt::blue)); // Blue color
    blueObstacle2->setPos(900, 450); // x, y coordinates in the scene
    scene->addItem(blueObstacle2);
    
    // Initialize camera position
    cameraX = player->pos().x() + player->rect().width() / 2;
    cameraY = scene->sceneRect().height() / 2;

    // Set up 60fps timer
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, [=]() {
        time += 1.0 / 60.0;  // Update time by 1/60th of a second for 60fps
        uiManager->updateTime(time);

        // Update score
        score += 1;
        uiManager->updateScore(score);

        // Update camera to follow player
        updateCamera();

        update();  // Trigger the paintEvent to refresh the display
    });

    gameTimer->start(1000 / 60);  // Update every 1/60 seconds for 60fps
}

MainWindow::~MainWindow()
{
    delete ui;
    delete uiManager;
    delete health;  // Clean up the Health object
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // Optionally, you can perform custom rendering here
}

void MainWindow::updatePlayerHealth(int newHealth)
{
    // Update the health display in the UI using UIManager
    uiManager->updateHealth(newHealth);
}

void MainWindow::updateCamera()
{
    if (!player)
        return;
        
    // Get player's center position
    QPointF playerPos = player->pos();
    qreal playerCenterX = playerPos.x() + player->rect().width() / 2;
    qreal playerCenterY = playerPos.y() + player->rect().height() / 2;
    
    // Get view dimensions
    QRectF viewRect = view->viewport()->rect();
    QRectF sceneRect = view->scene()->sceneRect();
    
    // Calculate target camera position with horizontal following only
    // Keep vertical position fixed in the middle of the screen
    qreal targetX = playerCenterX;
    qreal targetY = sceneRect.height() / 2;
    
    // Add boundaries to prevent camera from showing beyond the scene
    targetX = qMax(viewRect.width() / 2, targetX);
    targetX = qMin(targetX, sceneRect.right() - viewRect.width() / 2);
    
    // Initialize camera position on first run
    if (cameraX == 0.0 && cameraY == 0.0) {
        cameraX = targetX;
        cameraY = targetY;
    }
    
    // Smooth camera movement using interpolation
    // This creates a "lerp" effect where the camera follows with a slight delay
    cameraX += (targetX - cameraX) * cameraSmoothness;
    cameraY += (targetY - cameraY) * cameraSmoothness;
    
    // Round camera position to nearest pixel to prevent subpixel jittering
    qreal roundedX = qRound(cameraX);
    qreal roundedY = qRound(cameraY);
    
    // Set the view center with the smoothed position
    view->centerOn(roundedX, roundedY);
    
    // Position UI elements in the top right corner of the viewport
    int rightMargin = 20; // Distance from right edge
    int topMargin = 10;   // Distance from top edge
    int labelSpacing = 30; // Vertical space between labels
    
    // Calculate right-aligned positions based on viewport width
    int viewportWidth = view->viewport()->width();
    
    // Position labels in top-right corner with right alignment
    scoreLabel->move(viewportWidth - scoreLabel->width() - rightMargin, topMargin);
    healthLabel->move(viewportWidth - healthLabel->width() - rightMargin, topMargin + labelSpacing);
    timeLabel->move(viewportWidth - timeLabel->width() - rightMargin, topMargin + labelSpacing * 2);
    
    // Make sure UI is always on top
    scoreLabel->raise();
    healthLabel->raise();
    timeLabel->raise();
}
