#ifndef STATICOBSTACLE_H
#define STATICOBSTACLE_H
#include "obstacle.h"
class StaticObstacle : public Obstacle
{
protected:
    bool isBouncy; //Does this obstacle cause the player to bounce?
    float bounceStrength;  //bounce force to apply
    bool isClimbable; //Can the obstacle be climbed?
    bool isPushable;  //Can the obstacle be pushed?
    float playerFriction;
public:
    StaticObstacle(int length = 50, int width = 50, int damage = 1, QGraphicsItem* parent = nullptr);
    void move () override;
    void collideWithPlayer (Player* Player) override;
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
    void deleteObstacle();
};

#endif // STATICOBSTACLE_H
