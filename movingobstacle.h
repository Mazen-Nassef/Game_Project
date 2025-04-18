#ifndef MOVINGOBSTACLE_H
#define MOVINGOBSTACLE_H

#include "obstacle.h"
#include <QTimer>
#include <QPointF>  // Required for QPointF to work

class MovingObstacle : public Obstacle
{
    Q_OBJECT

public:
    enum class MovementPattern { Linear, Patrol, Circular };

protected:
    float moveSpeed = 2.0f;           // How fast the obstacle moves
    bool isPatrolling = false;        // Does it move back and forth?
    MovementPattern pattern = MovementPattern::Linear;
    QPointF startPosition = {0,0};    // Starting position for movement
    QPointF endPosition = {0,0};      // End position (for patrolling)
    QTimer* moveTimer = nullptr;      // Timer for movement updates
    float angle = 0.0f;               // For circular motion
    float radius = 0.0f;              // For circular motion

public:
    // Constructor with parameters
    MovingObstacle(int length = 50, int width = 50, int damage = 1, QGraphicsItem* parent = nullptr);

    // Destructor
    virtual ~MovingObstacle() override;

    // Movement logic (will be handled by the timer)
    void move() override;

    // Collision handling with player
    void collideWithPlayer(Player* player) override;

    // Setter methods for various movement patterns
    void setMovementPattern(MovementPattern pattern);
    void setPatrolPoints(const QPointF& start, const QPointF& end);
    void setMovementSpeed(float speed);
    void setCircularMotion(float centerX, float centerY, float radius);

private slots:
    void updatePosition();  // Called by timer to update position based on pattern
};

#endif // MOVINGOBSTACLE_H
