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

// Forward declaration
class QGraphicsPathItem;

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
    void performAttack();

    void takeDamage(int amount = 1);
    void reset();
    int getHealth() const { return health.get(); }
    
    signals:
    void healthChanged(int newHealth);
    

private slots:
    void applyGravity();
    void removeAttack();
    void checkAttackCollision();

private:
    // Check for platform collisions and handle them
    bool checkPlatformCollisions();
    // Check if the player can drop through the current platform
    bool canDropThroughPlatform();
    // Create the crescent-shaped attack graphic
    QGraphicsPathItem* createAttackGraphic(float dirX, float dirY);
    // Update the position of the attack graphic to follow the player
    void updateAttackPosition();
    
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

    // Dash related variables
    QTimer *dashTimer = nullptr;
    bool isDashing = false;
    float dashX = 0, dashY = 0;
    
    // Attack related variables
    QTimer *attackTimer = nullptr;
    QTimer *attackCollisionTimer = nullptr;
    bool isAttacking = false;
    float attackDuration = 500; // Default half a second
    float attackLaunchMagnitude = 7; // How far the player gets launched after hitting something
    QGraphicsPathItem *attackGraphic = nullptr; // Visual representation of attack
    float attackDistance = 30; // How far the attack appears from the player
    float attackDirectionX = 0;
    float attackDirectionY = 0;
};

#endif // PLAYER_H
