#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QTimer>
#include "health.h"

class Player : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    Player();

    void keyPressEvent(QKeyEvent *event) override;

    // Movement
    void moveForward();
    void moveBackward();
    void jump();

    // Game Mechanics
    void takeDamage();
    void reset();

private slots:
    void applyGravity();

private:
    Health health;
    float speed;
    int length;
    int width;
    int jumpHeight;
    int yVelocity;

    QTimer *gravityTimer;
};

#endif // PLAYER_H
