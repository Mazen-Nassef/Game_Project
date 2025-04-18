#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "player.h"  // Include the Player class
#include "uimanager.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , uiManager(new UIManager())
{
    ui->setupUi(this);

    // Create the UIManager instance
    uiManager = new UIManager();  // Create a UIManager instance


    // Set default score and health
    score = 0;
    health = 100;
    time = 0.0 ;

    // Set the initial values in UIManager
    uiManager->updateScore(score);
    uiManager->updateHealth(health);

    // Step 1: Create the scene
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);  // Set the scene's boundaries (change these values as needed)

    // Step 2: Create the player object
    Player* player = new Player();

    // Step 3: Add the player to the scene
    scene->addItem(player);

    // Step 4: Create the view and set the scene
    QGraphicsView* view = ui->graphicsView;  // Assuming you've named the Graphics View in Qt Designer
    view->setScene(scene);

    // Optionally, you can set the scene size, background, etc.
    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::SmoothPixmapTransform);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

 //Set up game timer
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, [=]() {
        int elapsedSeconds = startTime.secsTo(QTime::currentTime());
        uiManager->updateTime(elapsedSeconds);
        update();  // Redraw the UI with updated time
    });

    startTime = QTime::currentTime();  // Start counting time
    gameTimer->start(1000);  // Update every 1 second

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        time += 1.0;
        update();  // This will call paintEvent to redraw the time
    });
    timer->start(1000);  // Trigger every 1000 milliseconds (1 second)


}

MainWindow::~MainWindow()
{
    delete ui;
    delete uiManager;
}

// I added this part to your code mazen take a look at it
// to be able to link my classes to yours
void MainWindow::setScore(int newScore) {
    score = newScore;
    uiManager->updateScore(score);
    update();  // Redraw the window
}

void MainWindow::setHealth(int newHealth) {
    health = newHealth;
    uiManager->updateHealth(health);
    update();  // Redraw the window
}



void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // Set up font for score and health
    QFont font("Arial", 16, QFont::Bold);
    painter.setFont(font);
    painter.setPen(Qt::black);

    // Draw the score at the top-left corner
    painter.drawText(10, 30, "Score: " + QString::number(score));

    // Draw the health at the top-left corner below the score
    painter.drawText(10, 60, "Health: " + QString::number(health));

    painter.drawText(10, 90, "Time: " + QString::number(time, 'f', 2) + "s");

    // Draw the current message
    if (!currentMessage.isEmpty()) {
        painter.setPen(Qt::red);  // You can change the color of the message text
        painter.drawText(10, 100, currentMessage);
    }


}
