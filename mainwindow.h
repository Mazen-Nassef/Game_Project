#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include "player.h"
#include "uimanager.h"
#include <QTimer>
#include <QTime>


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

private:
    Ui::MainWindow *ui;
    UIManager* uiManager;
    int score;
    int health;
    double time;
    QTimer* timer;
     QString currentMessage;

    QTimer* gameTimer;
    QTime startTime;
};
#endif // MAINWINDOW_H
