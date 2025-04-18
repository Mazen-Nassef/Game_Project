#include "staticobstacle.h"
#include <QBrush>
#include <QGraphicsScene>

// Constructor: initialize base Obstacle and own properties
StaticObstacle::StaticObstacle(int length_, int width_, int damage_, QGraphicsItem* parent)
    : Obstacle(length_, width_, damage_, /*lethal=*/true,
               /*breakable=*/false, /*breakHP=*/0,
               /*color=*/Qt::gray,
               /*effect=*/false, /*effType=*/0, /*effDuration=*/0, //ignore the special obstacles for now
               parent),
    isBouncy(false),
    bounceStrength(0.0f),
    isClimbable(false),
    isPushable(false),
    playerFriction(1.0f)
{
    // Optionally adjust appearance for static obstacles
    // setBrush(QBrush(Qt::darkGray));
}

// Static obstacles do not move
void StaticObstacle::move()
{
}

void StaticObstacle::collideWithPlayer(Player* player)
{
    Q_UNUSED(player);
    // Do nothing for now
}

bool StaticObstacle::getIsBouncy() const
{
    return isBouncy;
}

void StaticObstacle::setIsBouncy(bool value)
{
    isBouncy = value;
}

float StaticObstacle::getBounceStrength() const
{
    return bounceStrength;
}

void StaticObstacle::setBounceStrength(float value)
{
    bounceStrength = value;
}

bool StaticObstacle::getIsClimbable() const
{
    return isClimbable;
}

void StaticObstacle::setIsClimbable(bool value)
{
    isClimbable = value;
}

bool StaticObstacle::getIsPushable() const
{
    return isPushable;
}

void StaticObstacle::setIsPushable(bool value)
{
    isPushable = value;
}

float StaticObstacle::getPlayerFriction() const
{
    return playerFriction;
}

void StaticObstacle::setPlayerFriction(float value)
{
    playerFriction = value;
}
