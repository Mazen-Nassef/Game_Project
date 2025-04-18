#include "movingobstacle.h"
#include "obstacle.h"
#include "qvectornd.h"
#include <QGraphicsScene>
#include <QtMath> // for qCos, qSin
#include "player.h" // Add include for Player

MovingObstacle::MovingObstacle(int length, int width, int damage, QGraphicsItem* parent)
    : Obstacle(length, width, damage, true, false, 0, Qt::gray, false, 0, 0, parent)
{
    startPosition = pos(); // Default starting point is current position

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &MovingObstacle::updatePosition);
    moveTimer->start(16); // ~60 updates per second
}

void MovingObstacle::collideWithPlayer(Player* player)
{
    if (player && getDamage() > 0) {
        player->takeDamage(getDamage());
    }
    
    // Moving obstacles can also push the player in their movement direction
    // This would require more complex handling and is optional for basic functionality
}


void MovingObstacle::setPatrolPoints(const QPointF& start, const QPointF& end)
{
    startPosition = start;
    endPosition = end;
    setPos(startPosition);
    isPatrolling = true;
}

void MovingObstacle::setMovementSpeed(float speed)
{
    moveSpeed = speed;
}

void MovingObstacle::setCircularMotion(float centerX, float centerY, float radiusValue)
{
    startPosition = {centerX, centerY};
    radius = radiusValue;
    angle = 0.0f;
}

void MovingObstacle::move()
{
    // This function could be triggered manually too, but main movement is done via updatePosition()
    updatePosition();
}


void MovingObstacle::updatePosition()
{
    switch (pattern)
    {
    case MovementPattern::Linear:
    {
        // Linear motion to the right
        setX(x() + moveSpeed);
        break;
    }
    case MovementPattern::Patrol:
    {
        // Move back and forth between startPosition and endPosition
        QPointF direction = (endPosition - startPosition);
        float totalDistance = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());

        if (totalDistance == 0) break; // avoid division by zero

        QPointF unitDirection = direction / totalDistance;

        setPos(pos() + unitDirection * moveSpeed);

        // If past the end point, reverse direction
        QPointF toEnd = endPosition - pos();
        QPointF toStart = startPosition - pos();

        if (QVector2D(toEnd).length() < moveSpeed || QVector2D(toStart).length() < moveSpeed)
        {
            // Swap start and end
            QPointF temp = startPosition;
            startPosition = endPosition;
            endPosition = temp;
        }

        break;
    }
    case MovementPattern::Circular:
    {
        if (radius == 0.0f) break;

        angle += moveSpeed / radius; // moveSpeed is interpreted as angular speed * radius

        if (angle > 360.0f)
            angle -= 360.0f;

        float rad = qDegreesToRadians(angle);
        float centerX = startPosition.x();
        float centerY = startPosition.y();

        float newX = centerX + radius * qCos(rad);
        float newY = centerY + radius * qSin(rad);

        setPos(newX, newY);

        break;
    }
    }
}
