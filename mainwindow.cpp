#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "player.h"
#include "uimanager.h"
#include "level.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , uiManager(new UIManager(this))
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
    delete currentLevel;
    
    // Scene will be cleaned up by Qt's parent-child system
}

void MainWindow::setupGame()
{
    // Set up UI
    uiManager->setupUI(this);
    
    // Connect UI signals
    connect(uiManager, &UIManager::levelSwitchRequested, 
            this, &MainWindow::switchToLevel);
    connect(uiManager, &UIManager::cameraUpdateRequested,
            this, &MainWindow::updateCamera);

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
    
    // Connect player to UI manager
    uiManager->connectPlayerSignals(player);
    
    // Initialize camera position
    cameraX = player->pos().x() + player->rect().width() / 2;
    cameraY = scene->sceneRect().height() / 2;

    // Start game timer in UI manager
    uiManager->startGameTimer();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // Custom rendering if needed
}

void MainWindow::switchToLevel(int levelType)
{
    // Set level type and create it
    currentLevel->setLevelType(static_cast<Level::LevelType>(levelType));
    currentLevel->createLevel();
    
    // Get the new player from the level
    player = currentLevel->getPlayer();
    
    // Connect player to UI manager
    uiManager->connectPlayerSignals(player);
    
    // Reset UI
    uiManager->resetUI();
    
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
    
    // Let UI manager position the labels
    uiManager->positionLabels(view);
}
