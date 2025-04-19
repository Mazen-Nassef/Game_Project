#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include "platform.h"
#include "staticobstacle.h"
#include "movingobstacle.h"
#include "player.h"

class Level {
public:
    enum LevelType {
        LEVEL_1,
        LEVEL_2
    };

private:
    int level_number;
    double frequency_multiplier;
    double velocity_multiplier;
    double damage_multiplier;
    int width;
    int height;
    LevelType level_type;

    void set_frequencyM();
    void set_velocityM();
    void set_damageM();

    // Level elements
    QGraphicsScene* scene;
    QList<Platform*> platforms;
    QList<Obstacle*> obstacles;
    QGraphicsPixmapItem* goal;
    
    // Clear level elements
    void clearLevel();
    
    // Level setup methods
    void setupLevel1();
    void setupLevel2();

public:
    Level(QGraphicsScene* scene = nullptr);
    ~Level();
    
    // Configure the level
    void setLevelType(LevelType type);
    void setScene(QGraphicsScene* scene);
    
    // Create the level with the current configuration
    void createLevel();
    
    // Level properties getters
    int getLevelNumber() const;
    double getFrequencyM() const;
    double getVelocityM() const;
    double getDamageM() const;
    int getWidth() const;
    int getHeight() const;
    
    // Update to next level
    void update_level();

    // Camera control
    void followPlayer(QGraphicsView* view, Player* player);
};

#endif // LEVEL_H
