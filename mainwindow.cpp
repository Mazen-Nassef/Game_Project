#include "mainwindow.h"
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

    scoreLabel->setStyleSheet("color: black; font: bold 16px;");
    healthLabel->setStyleSheet("color: black; font: bold 16px;");
    timeLabel->setStyleSheet("color: black; font: bold 16px;");

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
    scene->setSceneRect(0, 0, 800, 600);

    // Create the player object
    Player* player = new Player();
    scene->addItem(player);

    // Create the view and set the scene
    QGraphicsView* view = ui->graphicsView;
    view->setScene(scene);

    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::SmoothPixmapTransform);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Set up 60fps timer
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, [=]() {
        time += 1.0 / 60.0;  // Update time by 1/60th of a second for 60fps
        uiManager->updateTime(time);

        // You can update score/health here or use your game logic to modify them
        // Example: increase score as a test
        score += 1;
        uiManager->updateScore(score);

        // Decrease health every frame (if needed), for example:
        if (time > 5) {  // Health takes damage after 5 seconds as an example
            health->takeDamage(1);
        }
        uiManager->updateHealth(health->get());  // Use Health's currentHealth

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
