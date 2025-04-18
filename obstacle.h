#ifndef OBSTACLE_H
#define OBSTACLE_H
#include "qbrush.h"
#include <QObject>
#include <QGraphicsRectItem>
#include <QColor>
//#include <QKeyEvent>
//#include <QBrush>
class Player;
class Obstacle : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
protected:
    int length = 50;
    int width = 50;
    int damage = 1;
    bool isLethal = true; //causes immediate loss of all lives.
    bool isBreakable = false; //can be broken using the dash
    int breakHitpoints = 0;
    QColor color = Qt::gray;
    bool hasEffect = false; //can slow the player
    int effectType = 0;
    int effectDuration = 0;
public:
    Obstacle(QGraphicsItem* parent = nullptr);
    Obstacle(int length_          = 50,
             int width_           = 50,
             int damage_          = 1,
             bool lethal          = true,
             bool breakable       = false,
             int  breakHP         = 0,
             QColor col          = Qt::gray,
             bool effect         = false,
             int  effType        = 0,
             int  effDuration    = 0,
             QGraphicsItem* parent = nullptr): QObject(),
          QGraphicsRectItem(parent),
          length(length_),
          width(width_),
          damage(damage_),
          isLethal(lethal),
          isBreakable(breakable),
          breakHitpoints(breakHP),
          color(col),
          hasEffect(effect),
          effectType(effType),
          effectDuration(effDuration)  {
        setRect(0,0, length, width);
        setBrush(color);
    }

              virtual void collideWithPlayer(Player* player);
              int getDamage() const;
              virtual void takeDamage(int amount);
              virtual ~Obstacle();
              virtual void move() = 0;
};

#endif // OBSTACLE_H
