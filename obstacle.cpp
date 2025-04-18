#include "obstacle.h"
#include "player.h"

// Constructor implementation
Obstacle::Obstacle(QGraphicsItem *parent) : QObject(),
    QGraphicsRectItem(parent)
{
    setRect(0, 0, length, width);
    setBrush(color);
}

// Destructor implementation
Obstacle::~Obstacle()
{
    // Default destructor
}

// Implementation of collideWithPlayer
void Obstacle::collideWithPlayer(Player* player)
{
    // Default implementation - can be overridden by derived classes
    if (player) {
        player->takeDamage();
    }
}

// Implementation of getDamage
int Obstacle::getDamage() const
{
    return damage;
}

// Implementation of takeDamage
void Obstacle::takeDamage(int amount)
{
    // Default implementation - can be overridden by derived classes
    if (isBreakable) {
        breakHitpoints -= amount;
        
        // If hitpoints drop to or below 0, the obstacle is destroyed
        if (breakHitpoints <= 0) {
            // Code to destroy the obstacle
            // This will depend on your game's implementation
            // For now, we can hide it
            hide();
        }
    }
}