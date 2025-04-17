#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "player.h"  // Include the Player class

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

}

MainWindow::~MainWindow()
{
    delete ui;
}
