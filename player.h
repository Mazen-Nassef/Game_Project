#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QTimer>
#include <Qset>
#include <QDebug>
#include "health.h"
#include "platform.h"

class Player : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    Player();

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


    void moveForward();
    void moveBackward();
    void jump();
    void performDash();

    void takeDamage(int amount = 1);
    void reset();

private slots:
    void applyGravity();

private:
    // Check for platform collisions and handle them
    bool checkPlatformCollisions();
    // Check if the player can drop through the current platform
    bool canDropThroughPlatform();
    
    Health health;
    float speed;
    float maxSpeed;
    float friction;
    float gravity;
    float dashSpeed;
    int dashDuration;
    int length;
    int width;
    int jumpHeight;
    int yVelocity;
    float xVelocity;
    bool canDash;
    bool isOnGround;
    bool isOnPlatform;
    Platform* currentPlatform;
    qreal floorY; // Y coordinate of the floor


    QTimer *gravityTimer;

    QSet<int> keysHeld;

    QTimer *dashTimer = nullptr;
    bool isDashing = false;
    float dashX = 0, dashY = 0;

};

#endif // PLAYER_H
