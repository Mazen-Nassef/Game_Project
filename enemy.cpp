#include "enemy.h"
#include "level.h"
#include "qgraphicsscene.h"
#include <QBrush>

// Constructor for Enemy object
Enemy::Enemy(QGraphicsItem *parent)
    : Obstacle(50, 50, 0, false, false, 0, Qt::red, false, 0, 0, false, false, 0.0f,
               false, false, 1.0f, MovementPattern::Linear, 0.0f, QPointF(), QPointF(), 0.0f, parent),
    health(1) // Initialize with 1 HP; adjust if needed
{
    setBrush(QBrush(Qt::red)); // Optional visual distinction
}

void Enemy::takeDamage(int amount)
{
    health.takeDamage(amount);
    if (isDead()) {
        setPos(0,0);
    }
}

// Check if the enemy's health is 0 or below
bool Enemy::isDead() const
{
    return health.get() <= 0;
}
