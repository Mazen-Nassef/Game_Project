#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include "player.h"
#include "uimanager.h"
#include "health.h"
#include "level.h"
#include <QTimer>
#include <QTime>
#include <QLabel>

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

    // Setters for score and health
    void setScore(int score);
    void setHealth(int health);
    int getElapsedTime() const;

protected:
    // Paint event to update the UI
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updatePlayerHealth(int newHealth);
    void switchToLevel(Level::LevelType levelType);

private:
    // Camera method to update view position
    void updateCamera();
    void setupGame();
    
    Ui::MainWindow *ui;
    UIManager* uiManager;
    int score;
    Health* health;
    double time;
    QTimer* timer;
    QString currentMessage;

    QTimer* gameTimer;
    QTime startTime;

    QLabel *scoreLabel;
    QLabel *healthLabel;
    QLabel *timeLabel;
    
    // Game elements
    QGraphicsScene* scene;
    Level* currentLevel;
    Player* player;
    QGraphicsView* view;
    
    // Camera position tracking for smooth movement
    qreal cameraX = 0.0;
    qreal cameraY = 0.0;
    qreal cameraSmoothness = 0.1; // Lower = smoother but slower camera (0.1-0.3 is good)
};
#endif // MAINWINDOW_H
