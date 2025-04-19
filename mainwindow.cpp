#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "player.h"
#include "uimanager.h"
#include "health.h"
#include "level.h"
#include <QTimer>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , uiManager(new UIManager())
    , health(new Health(3))
    , scene(nullptr)
    , currentLevel(nullptr)
    , player(nullptr)
{
    ui->setupUi(this);
    setupGame();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete uiManager;
    delete health;
    delete currentLevel;
    
    // Scene and player will be cleaned up by Qt's parent-child system
}

void MainWindow::setupGame()
{
    // Set up UI labels
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
    uiManager->updateHealth(health->get());

    // Create the scene
    scene = new QGraphicsScene(this);
    
    // Create the view and set the scene
    view = ui->graphicsView;
    view->setScene(scene);

    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::SmoothPixmapTransform);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // Create level manager and set it up with the current scene
    currentLevel = new Level(scene);
    currentLevel->setLevelType(Level::LEVEL_1);
    currentLevel->createLevel();
    
    // Get the player from level
    player = currentLevel->getPlayer();
    
    // Connect player's health changes to the UI update
    connect(player, &Player::healthChanged, this, &MainWindow::updatePlayerHealth);
    
    // Set the initial health display
    uiManager->updateHealth(player->getHealth());
    
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
    
    // Add level switch buttons for testing
    QPushButton* level1Button = new QPushButton("Level 1", this);
    QPushButton* level2Button = new QPushButton("Level 2", this);
    
    connect(level1Button, &QPushButton::clicked, this, [this]() {
        switchToLevel(Level::LEVEL_1);
    });
    
    connect(level2Button, &QPushButton::clicked, this, [this]() {
        switchToLevel(Level::LEVEL_2);
    });
    
    // Position the buttons
    level1Button->move(10, this->height() - 40);
    level2Button->move(90, this->height() - 40);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // Custom rendering if needed
}

void MainWindow::updatePlayerHealth(int newHealth)
{
    // Update the health display in the UI using UIManager
    uiManager->updateHealth(newHealth);
}

void MainWindow::switchToLevel(Level::LevelType levelType)
{
    // Set level type and create it
    currentLevel->setLevelType(levelType);
    currentLevel->createLevel();
    
    // Get the new player from the level
    player = currentLevel->getPlayer();
    
    // Connect player's health changes to the UI update
    connect(player, &Player::healthChanged, this, &MainWindow::updatePlayerHealth);
    
    // Reset score and time
    score = 0;
    time = 0.0;
    uiManager->updateScore(score);
    uiManager->updateTime(time);
    uiManager->updateHealth(player->getHealth());
    
    // Reset camera
    cameraX = player->pos().x() + player->rect().width() / 2;
    cameraY = scene->sceneRect().height() / 2;
    view->centerOn(cameraX, cameraY);
}

void MainWindow::updateCamera()
{
    if (!currentLevel)
        return;
    
    // Let the level handle camera movement
    currentLevel->followPlayer(view);
    
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
