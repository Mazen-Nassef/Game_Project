#ifndef MOVINGOBSTACLE_H
#define MOVINGOBSTACLE_H
#include "obstacle.h"
#include <QTimer>
class MovingObstacle : public Obstacle
{
    Q_OBJECT
public:
    enum class MovementPattern { Linear, Patrol, Circular };
protected:
    float moveSpeed = 2.0f;           // How fast the obstacle moves
    bool isPatrolling = false;        // Does it move back and forth
    MovementPattern pattern = MovementPattern::Linear;
    QPointF startPosition = {0,0};     // Starting position
    QPointF endPosition = {0,0};       // End position (for patrolling)
    QTimer* moveTimer = nullptr;         // Timer for movement updates
    // For circular motion:
    float angle = 0.0f;
    float radius = 0.0f;
public:

    MovingObstacle(int length = 50, int width = 50, int damage = 1, QGraphicsItem* parent = nullptr);
    virtual ~MovingObstacle() override = default;
    void move () override;
    void collideWithPlayer(Player* Player) override;
    void setMovementPattern(MovementPattern pattern) {this->pattern = pattern;}
    void setPatrolPoints(const QPointF& start, const QPointF& end);
    void setMovementSpeed(float speed);
    void setCircularMotion(float centerX, float centerY, float radius);
private slots:
    void updatePosition();     // Called by timer to update position
};
#endif // MOVINGOBSTACLE_H
