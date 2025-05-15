#ifndef ENEMY_H
#define ENEMY_H

#include "obstacle.h"
#include "health.h"

class Enemy : public Obstacle
{
    Q_OBJECT
public:
    explicit Enemy(QGraphicsItem *parent = nullptr);
    void takeDamage(int amount); // Override to apply damage to health
    bool isDead() const;         // To check if health has reached 0

private:
    Health health;
    bool isInvulnerable;
    QTimer* invulnerabilityTimer;
};

#endif // ENEMY_H
