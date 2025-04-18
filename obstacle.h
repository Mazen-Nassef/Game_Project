#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QColor>
#include <QBrush>

class Player;

class Obstacle : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

protected:
    int length;
    int width;
    int damage;
    bool isLethal;
    bool isBreakable;
    int breakHitpoints;
    QColor color;
    bool hasEffect;
    int effectType;
    int effectDuration;

public:
    Obstacle(QGraphicsItem* parent = nullptr);
    Obstacle(int length_,
             int width_,
             int damage_,
             bool lethal,
             bool breakable,
             int breakHP,
             QColor col,
             bool effect,
             int effType,
             int effDuration,
             QGraphicsItem* parent = nullptr);

    virtual ~Obstacle();

    virtual void collideWithPlayer(Player* player);
    int getDamage() const;
    virtual void takeDamage(int amount);
    virtual void move() = 0;
};

#endif // OBSTACLE_H
