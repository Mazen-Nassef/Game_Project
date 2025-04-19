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
    bool isStompable;  // Can the player destroy this obstacle by stomping on it?

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
    
    // Getter and setter for isStompable
    bool getIsStompable() const;
    void setIsStompable(bool value);
};

#endif // OBSTACLE_H
