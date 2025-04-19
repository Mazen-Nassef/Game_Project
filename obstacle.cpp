#include "obstacle.h"
#include "player.h"
#include "qgraphicsscene.h"

Obstacle::Obstacle(QGraphicsItem *parent)
    : QObject(), QGraphicsRectItem(parent),
    length(50), width(50), damage(1), isLethal(true),
    isBreakable(false), breakHitpoints(0),
    color(Qt::gray), hasEffect(false),
    effectType(0), effectDuration(0), isStompable(false)
{
    setRect(0, 0, length, width);
    setBrush(QBrush(color));
}

Obstacle::Obstacle(int length_,
                  int width_,
                   int damage_,
                   bool lethal,
                   bool breakable,
                   int breakHP,
                   QColor col,
                   bool effect,
                   int effType,
                   int effDuration,
                   QGraphicsItem* parent)
    : QObject(), QGraphicsRectItem(parent),
    length(length_), width(width_), damage(damage_),
    isLethal(lethal), isBreakable(breakable), breakHitpoints(breakHP),
    color(col), hasEffect(effect), effectType(effType),
    effectDuration(effDuration), isStompable(false)
{
    setRect(0, 0, length, width);
    setBrush(QBrush(color));
}

Obstacle::~Obstacle()
{
    // Destructor body
}

void Obstacle::collideWithPlayer(Player* player)
{
    if (player && damage > 0) {
        player->takeDamage(damage);
    }
}

int Obstacle::getDamage() const
{
    return damage;
}

void Obstacle::takeDamage(int amount)
{
    if (isBreakable) {
        breakHitpoints -= amount;
        if (breakHitpoints <= 0) {
            // Remove from scene and delete
            if (scene()) {
                scene()->removeItem(this);
            }
            deleteLater();
        }
    }
}

bool Obstacle::getIsStompable() const
{
    return isStompable;
}

void Obstacle::setIsStompable(bool value)
{
    isStompable = value;
}
