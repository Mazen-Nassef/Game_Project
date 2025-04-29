#include "obstacle.h"
#include "player.h"
#include <QBrush>
#include <QGraphicsScene>

Obstacle::Obstacle(QGraphicsItem *parent)
    : QObject(), QGraphicsRectItem(parent),
    length(50), width(50), damage(1), isLethal(true),
    isBreakable(false), breakHitpoints(0),
    color(Qt::gray), hasEffect(false),
    effectType(0), effectDuration(0), isStompable(false),
    isBouncy(false), bounceStrength(0.0f),
    isClimbable(false), isPushable(false), playerFriction(1.0f),
    moveSpeed(2.0f), isPatrolling(false), pattern(MovementPattern::Linear),
    startPosition(0,0), endPosition(0,0), moveTimer(nullptr),
    angle(0.0f), radius(0.0f)
{
    setRect(0, 0, length, width);
    setBrush(QBrush(color));
}

Obstacle::Obstacle(int length_, int width_, int damage_, bool lethal,
                   bool breakable, int breakHP, QColor col,
                   bool effect, int effType, int effDuration,
                   bool stompable, bool bouncy, float bounceStr,
                   bool climbable, bool pushable, float friction,
                   MovementPattern pattern_, float speed,
                   const QPointF &startPos, const QPointF &endPos,
                   float circularRadius, QGraphicsItem *parent)
    : QObject(), QGraphicsRectItem(parent),
    length(length_), width(width_), damage(damage_),
    isLethal(lethal), isBreakable(breakable), breakHitpoints(breakHP),
    color(col), hasEffect(effect), effectType(effType),
    effectDuration(effDuration), isStompable(stompable),
    isBouncy(bouncy), bounceStrength(bounceStr),
    isClimbable(climbable), isPushable(pushable), playerFriction(friction),
    moveSpeed(speed), isPatrolling(pattern_==MovementPattern::Patrol), pattern(pattern_),
    startPosition(startPos), endPosition(endPos), angle(0.0f), radius(circularRadius)
{
    setRect(0, 0, length, width);
    setBrush(QBrush(color));
    if (pattern != MovementPattern::Linear) {
        moveTimer = new QTimer(this);
        connect(moveTimer, &QTimer::timeout, this, &Obstacle::updatePosition);
        moveTimer->start(16);
    }
}

Obstacle::~Obstacle()
{
}

void Obstacle::collideWithPlayer(Player *player)
{
    if (player && damage > 0) {
        player->takeDamage(damage);
    }
}

int Obstacle::getDamage() const { return damage; }

void Obstacle::takeDamage(int amount)
{
    if (isBreakable) {
        breakHitpoints -= amount;
        if (breakHitpoints <= 0) {
            if (scene()) scene()->removeItem(this);
            deleteLater();
        }
    }
}

bool Obstacle::getIsStompable() const { return isStompable; }
void Obstacle::setIsStompable(bool value) { isStompable = value; }

bool Obstacle::getIsBouncy() const { return isBouncy; }
void Obstacle::setIsBouncy(bool value) { isBouncy = value; }

float Obstacle::getBounceStrength() const { return bounceStrength; }
void Obstacle::setBounceStrength(float value) { bounceStrength = value; }

bool Obstacle::getIsClimbable() const { return isClimbable; }
void Obstacle::setIsClimbable(bool value) { isClimbable = value; }

bool Obstacle::getIsPushable() const { return isPushable; }
void Obstacle::setIsPushable(bool value) { isPushable = value; }

float Obstacle::getPlayerFriction() const { return playerFriction; }
void Obstacle::setPlayerFriction(float value) { playerFriction = value; }

void Obstacle::deleteObstacle()
{
    if (scene()) {
        scene()->removeItem(this);
    }
    deleteLater();  // Schedules it for safe deletion
}


void Obstacle::move() //The move function for moving obstacles.
{
    switch (pattern) {
    case MovementPattern::Linear:
        setPos(x() + moveSpeed, y());
        break;
    case MovementPattern::Patrol:
        // simple back-and-forth
        if (isPatrolling) {
            setPos(x() + moveSpeed, y());
            if (x() >= endPosition.x() || x() <= startPosition.x()) moveSpeed = -moveSpeed;
        }
        break;
    case MovementPattern::Circular:
        angle += moveSpeed;
        setPos(startPosition.x() + radius * qCos(angle), startPosition.y() + radius * qSin(angle));
        break;
    }
}

void Obstacle::updatePosition()
{
    move();
}

void Obstacle::setMovementPattern(MovementPattern pat) // A setter for the movement pattern of moving obstacles.
{
    pattern = pat;
    isPatrolling = (pat == MovementPattern::Patrol);
}

void Obstacle::setPatrolPoints(const QPointF &start, const QPointF &end) // A setter for the starting point and ending point
                                                                        //for obstacles moving in a patrolling pattern.
{
    startPosition = start;
    endPosition = end;
}

void Obstacle::setMovementSpeed(float speedVal) { moveSpeed = speedVal; } // For moving obstacles as a whole.

void Obstacle::setCircularMotion(float centerX, float centerY, float rad) // For obstacles moving in a circular pattern.
{
    startPosition = QPointF(centerX, centerY);
    radius = rad;
    pattern = MovementPattern::Circular;
}

