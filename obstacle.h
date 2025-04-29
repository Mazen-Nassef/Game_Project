#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QColor>
#include <QBrush>
#include <QTimer>
#include <QPointF>

class Player;

class Obstacle : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    enum class MovementPattern { Linear, Patrol, Circular };

protected:
    int length;
    int width;
    int damage;
    bool isLethal; // once the player steps on the obstacle, it kills him
    bool isBreakable; // the player can break the obstacle by dashing
    int breakHitpoints;
    QColor color;
    bool hasEffect; // the obstacle can slow down the player (e.g slows him down)
    int effectType;
    int effectDuration;
    bool isStompable; // the player destroys the obstacle upon jumping on it

    bool isBouncy;
    float bounceStrength;
    bool isClimbable;
    bool isPushable;
    float playerFriction;

    float moveSpeed;
    bool isPatrolling;
    MovementPattern pattern;
    QPointF startPosition;
    QPointF endPosition;
    QTimer* moveTimer;
    float angle;
    float radius;

public:
    Obstacle(QGraphicsItem* parent = nullptr);
    Obstacle(int length,
             int width,
             int damage,
             bool lethal,
             bool breakable,
             int breakHP,
             QColor col,
             bool effect,
             int effType,
             int effDuration,
             bool stompable,
             bool bouncy,
             float bounceStr,
             bool climbable,
             bool pushable,
             float friction,
             MovementPattern pattern = MovementPattern::Linear,
             float speed = 2.0f,
             const QPointF& startPos = QPointF(0,0),
             const QPointF& endPos = QPointF(0,0),
             float circularRadius = 0.0f,
             QGraphicsItem* parent = nullptr);
    virtual ~Obstacle();

    void collideWithPlayer(Player* player);
    int getDamage() const;
    void takeDamage(int amount);
    void move();

    bool getIsStompable() const;
    void setIsStompable(bool value);
    bool getIsBouncy() const;
    void setIsBouncy(bool value);
    float getBounceStrength() const;
    void setBounceStrength(float value);
    bool getIsClimbable() const;
    void setIsClimbable(bool value);
    bool getIsPushable() const;
    void setIsPushable(bool value);
    float getPlayerFriction() const;
    void setPlayerFriction(float value);

    void setMovementPattern(MovementPattern pat);
    void setPatrolPoints(const QPointF& start, const QPointF& end);
    void setMovementSpeed(float speedVal);
    void setCircularMotion(float centerX, float centerY, float rad);

private slots:
    void updatePosition();
};

#endif // OBSTACLE_H

