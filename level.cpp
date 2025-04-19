#include "level.h"
#include <QGraphicsView>
#include <QBrush>

Level::Level(QGraphicsScene* scene) {
    this->scene = scene;
    level_number = 1;
    level_type = LEVEL_1;
    width = 2000;
    height = 600;
    goal = nullptr;
    
    set_frequencyM();
    set_velocityM();
    set_damageM();
}

Level::~Level() {
    clearLevel();
}

void Level::clearLevel() {
    if (!scene) return;
    
    // Clear platforms
    for (auto platform : platforms) {
        scene->removeItem(platform);
        delete platform;
    }
    platforms.clear();
    
    // Clear obstacles
    for (auto obstacle : obstacles) {
        scene->removeItem(obstacle);
        delete obstacle;
    }
    obstacles.clear();
    
    // Clear goal
    if (goal) {
        scene->removeItem(goal);
        delete goal;
        goal = nullptr;
    }
}

void Level::setScene(QGraphicsScene* scene) {
    // Clear old scene if it exists
    if (this->scene) {
        clearLevel();
    }
    
    this->scene = scene;
    
    // Set scene dimensions
    if (scene) {
        scene->setSceneRect(0, 0, width, height);
    }
}

void Level::setLevelType(LevelType type) {
    level_type = type;
}

void Level::createLevel() {
    if (!scene) return;
    
    // Clear any existing level elements
    clearLevel();
    
    // Set scene dimensions
    scene->setSceneRect(0, 0, width, height);
    
    // Create level based on type
    switch (level_type) {
        case LEVEL_1:
            setupLevel1();
            break;
        case LEVEL_2:
            setupLevel2();
            break;
    }
}

void Level::setupLevel1() {
    // Create main floor platform (solid)
    Platform* floor = new Platform(0, 500, width, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(floor);
    platforms.append(floor);
    
    // Add some floating platforms (passthrough)
    Platform* platform1 = new Platform(200, 400, 200, 20);
    Platform* platform2 = new Platform(500, 350, 200, 20);
    Platform* platform3 = new Platform(800, 300, 200, 20);
    Platform* platform4 = new Platform(1100, 250, 200, 20);
    Platform* platform5 = new Platform(1400, 300, 200, 20);
    
    scene->addItem(platform1);
    scene->addItem(platform2);
    scene->addItem(platform3);
    scene->addItem(platform4);
    scene->addItem(platform5);
    
    platforms.append(platform1);
    platforms.append(platform2);
    platforms.append(platform3);
    platforms.append(platform4);
    platforms.append(platform5);
    
    // Create obstacles
    
    // Purple obstacles (single use)
    StaticObstacle* purpleObstacle1 = new StaticObstacle(50, 50, 1);
    purpleObstacle1->setIsBouncy(false);
    purpleObstacle1->setBounceStrength(0.0f);
    purpleObstacle1->setPlayerFriction(1.0f);
    purpleObstacle1->setBrush(QBrush(Qt::magenta)); // Purple color
    purpleObstacle1->setPos(300, 350); // x, y coordinates in the scene
    scene->addItem(purpleObstacle1);
    obstacles.append(purpleObstacle1);
    
    StaticObstacle* purpleObstacle2 = new StaticObstacle(100, 50, 1);
    purpleObstacle2->setIsBouncy(false);
    purpleObstacle2->setBounceStrength(0.0f);
    purpleObstacle2->setPlayerFriction(1.0f);
    purpleObstacle2->setBrush(QBrush(Qt::magenta)); // Purple color
    purpleObstacle2->setPos(700, 400); // x, y coordinates in the scene
    scene->addItem(purpleObstacle2);
    obstacles.append(purpleObstacle2);
    
    // Blue bouncy obstacles
    StaticObstacle* blueObstacle1 = new StaticObstacle(100, 50, 0); // No damage
    blueObstacle1->setIsBouncy(true);
    blueObstacle1->setBounceStrength(1.2f); // Very bouncy
    blueObstacle1->setPlayerFriction(0.8f);
    blueObstacle1->setBrush(QBrush(Qt::blue)); // Blue color
    blueObstacle1->setPos(500, 450); // x, y coordinates in the scene
    scene->addItem(blueObstacle1);
    obstacles.append(blueObstacle1);
    
    StaticObstacle* blueObstacle2 = new StaticObstacle(100, 50, 0); // No damage
    blueObstacle2->setIsBouncy(true);
    blueObstacle2->setBounceStrength(1.2f); // Very bouncy
    blueObstacle2->setPlayerFriction(0.8f);
    blueObstacle2->setBrush(QBrush(Qt::blue)); // Blue color
    blueObstacle2->setPos(900, 450); // x, y coordinates in the scene
    scene->addItem(blueObstacle2);
    obstacles.append(blueObstacle2);
    
    // Add a moving obstacle that patrols
    MovingObstacle* movingObstacle = new MovingObstacle(80, 40, 1);
    movingObstacle->setBrush(QBrush(Qt::red));
    movingObstacle->setMovementSpeed(2.0f);
    movingObstacle->setPatrolPoints(QPointF(600, 200), QPointF(900, 200));
    scene->addItem(movingObstacle);
    obstacles.append(movingObstacle);
    
    // Add goal
    goal = new QGraphicsPixmapItem(QPixmap(":/images/flag.png").scaled(50, 50));
    goal->setPos(width - 100, 450);  // end of the level
    scene->addItem(goal);
}

void Level::setupLevel2() {
    // Level 2 - harder layout with more obstacles
    
    // Create main floor platform (solid) but with gaps
    Platform* floor1 = new Platform(0, 500, 600, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    Platform* floor2 = new Platform(750, 500, 600, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    Platform* floor3 = new Platform(1500, 500, 500, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    
    scene->addItem(floor1);
    scene->addItem(floor2);
    scene->addItem(floor3);
    
    platforms.append(floor1);
    platforms.append(floor2);
    platforms.append(floor3);
    
    // Add some floating platforms (passthrough)
    Platform* platform1 = new Platform(200, 350, 150, 20);
    Platform* platform2 = new Platform(450, 300, 150, 20);
    Platform* platform3 = new Platform(650, 350, 200, 20);
    Platform* platform4 = new Platform(950, 300, 150, 20);
    Platform* platform5 = new Platform(1200, 250, 150, 20);
    Platform* platform6 = new Platform(1450, 300, 150, 20);
    Platform* platform7 = new Platform(1700, 350, 150, 20);
    
    scene->addItem(platform1);
    scene->addItem(platform2);
    scene->addItem(platform3);
    scene->addItem(platform4);
    scene->addItem(platform5);
    scene->addItem(platform6);
    scene->addItem(platform7);
    
    platforms.append(platform1);
    platforms.append(platform2);
    platforms.append(platform3);
    platforms.append(platform4);
    platforms.append(platform5);
    platforms.append(platform6);
    platforms.append(platform7);
    
    // Create more obstacles - more dangerous ones
    
    // Purple obstacles (single use)
    StaticObstacle* purpleObstacle1 = new StaticObstacle(70, 70, 2); // More damage
    purpleObstacle1->setIsBouncy(false);
    purpleObstacle1->setBrush(QBrush(Qt::magenta));
    purpleObstacle1->setPos(300, 280);
    scene->addItem(purpleObstacle1);
    obstacles.append(purpleObstacle1);
    
    StaticObstacle* purpleObstacle2 = new StaticObstacle(70, 70, 2);
    purpleObstacle2->setIsBouncy(false);
    purpleObstacle2->setBrush(QBrush(Qt::magenta));
    purpleObstacle2->setPos(1100, 230);
    scene->addItem(purpleObstacle2);
    obstacles.append(purpleObstacle2);
    
    // Blue bouncy obstacles
    StaticObstacle* blueObstacle1 = new StaticObstacle(120, 50, 0);
    blueObstacle1->setIsBouncy(true);
    blueObstacle1->setBounceStrength(1.5f); // Extra bouncy
    blueObstacle1->setBrush(QBrush(Qt::blue));
    blueObstacle1->setPos(500, 430);
    scene->addItem(blueObstacle1);
    obstacles.append(blueObstacle1);
    
    StaticObstacle* blueObstacle2 = new StaticObstacle(120, 50, 0);
    blueObstacle2->setIsBouncy(true);
    blueObstacle2->setBounceStrength(1.5f);
    blueObstacle2->setBrush(QBrush(Qt::blue));
    blueObstacle2->setPos(1300, 430);
    scene->addItem(blueObstacle2);
    obstacles.append(blueObstacle2);
    
    // Add multiple moving obstacles with different patterns
    
    // Circular motion obstacle
    MovingObstacle* circleObstacle = new MovingObstacle(60, 60, 2);
    circleObstacle->setBrush(QBrush(Qt::red));
    circleObstacle->setCircularMotion(800, 200, 100);
    circleObstacle->setMovementSpeed(3.0f);
    scene->addItem(circleObstacle);
    obstacles.append(circleObstacle);
    
    // Patrolling obstacles
    MovingObstacle* patrolObstacle1 = new MovingObstacle(100, 40, 1);
    patrolObstacle1->setBrush(QBrush(Qt::red));
    patrolObstacle1->setMovementSpeed(3.0f);
    patrolObstacle1->setPatrolPoints(QPointF(400, 250), QPointF(600, 250));
    scene->addItem(patrolObstacle1);
    obstacles.append(patrolObstacle1);
    
    MovingObstacle* patrolObstacle2 = new MovingObstacle(100, 40, 1);
    patrolObstacle2->setBrush(QBrush(Qt::red));
    patrolObstacle2->setMovementSpeed(4.0f);
    patrolObstacle2->setPatrolPoints(QPointF(1000, 200), QPointF(1300, 200));
    scene->addItem(patrolObstacle2);
    obstacles.append(patrolObstacle2);
    
    // Add goal
    goal = new QGraphicsPixmapItem(QPixmap(":/images/flag.png").scaled(50, 50));
    goal->setPos(width - 100, 450);
    scene->addItem(goal);
}

int Level::getLevelNumber() const {
    return level_number;
}

double Level::getDamageM() const {
    return damage_multiplier;
}

double Level::getFrequencyM() const {
    return frequency_multiplier;
}

double Level::getVelocityM() const {
    return velocity_multiplier;
}

int Level::getWidth() const {
    return width;
}

int Level::getHeight() const {
    return height;
}

void Level::update_level() {
    level_number++;
    set_frequencyM();
    set_velocityM();
    set_damageM();
    
    // Switch to level 2 if we're on level 1
    if (level_type == LEVEL_1) {
        level_type = LEVEL_2;
        createLevel();
    }
}

void Level::set_frequencyM() {
    frequency_multiplier = 1.0 + 0.2 * (level_number - 1);
}

void Level::set_velocityM() {
    velocity_multiplier = 1.0 + 0.15 * (level_number - 1);
}

void Level::set_damageM() {
    damage_multiplier = 1.0 + 0.1 * (level_number - 1);
}

void Level::followPlayer(QGraphicsView* view, Player* player) {
    if (!view || !player) return;
    
    int playerX = player->x();
    view->centerOn(playerX, 300); // vertical center is fixed
}
