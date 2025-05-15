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
        // Remove the enemy from the level's enemies list before deletion
        Level* level = dynamic_cast<Level*>(scene()); // Assuming you're using a Level class to manage the scene
        if (level) {
            level->removeEnemy(this); // Implement a method to remove enemies from the level's list
        }

        scene()->removeItem(this);
        this->deleteLater(); // Safely delete after the removal
    }
}

// Check if the enemy's health is 0 or below
bool Enemy::isDead() const
{
    return health.get() <= 0;
}
