#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTimer>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include "player.h"
#include "uimanager.h"
#include "level.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:


private slots:
    void switchToLevel(int levelType);
    void updateCamera();

private:
    void setupGame();
    
    Ui::MainWindow *ui;
    UIManager* uiManager;
    QTimer*           frameTimer; // to handle collision with flag
    // Game elements
    QGraphicsScene* scene;
    Level* currentLevel;
    Player* player; // Reference to the player owned by Level
    QGraphicsView* view;
    
    // Camera position tracking
    qreal cameraX = 0.0;
    qreal cameraY = 0.0;
};
#endif // MAINWINDOW_H
