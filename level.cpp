#include "level.h"
#include "enemy.h"
#include <QGraphicsView>
#include <QBrush>
#include <QPen>

Level::Level(QGraphicsScene* scene) {
    this->scene = scene;
    level_number = 1;
    level_type = LEVEL_1;
    goal = nullptr;
    player = nullptr;

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

    // Clear enemies and remove them from the scene
    for (auto enemy : enemies) {
        if (scene->items().contains(enemy)) {
            scene->removeItem(enemy);
        }
        removeEnemy(enemy); // Ensure enemy is removed from the enemies list
        enemy->deleteLater();
    }
    enemies.clear();

    // Clear goal
    if (goal) {
        scene->removeItem(goal);
        delete goal;
        goal = nullptr;
    }

    // Clear player
    if (player) {
        scene->removeItem(player);
        delete player;
        player = nullptr;
    }
}

void Level::removeEnemy(Enemy* enemy) {
    // Remove the enemy from the enemies list
    enemies.removeOne(enemy);
}

void Level::setScene(QGraphicsScene* scene) {
    // Clear old scene if it exists
    if (this->scene) {
        clearLevel();
    }

    this->scene = scene;
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
    case LEVEL_3:
        setupLevel3();
        break;
    case LEVEL_4:
        setupLevel4();
        break;
    case LEVEL_5:
        setupLevel5();
        break;
    }
}

void Level::setupLevel1() {
    width = 6000;
    height = 800;

    scene->setSceneRect(0, 0, width, height);

    player = new Player();
    player->setPos(100, 600);
    scene->addItem(player);
    player->setFocus();

    Platform* floor = new Platform(0, 700, 2000, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(floor);
    platforms.append(floor);

    Platform* raisedLedge = new Platform(1500, 600, 500, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(raisedLedge);
    platforms.append(raisedLedge);

    Platform* landingPlatform = new Platform(2400, 700, 1100, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(landingPlatform);
    platforms.append(landingPlatform);

    Platform* plateauBase = new Platform(3500, 300, 1500, 500, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(plateauBase);
    platforms.append(plateauBase);

    Platform* plateauEnd = new Platform(5000, 700, 1500, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(plateauEnd);
    platforms.append(plateauEnd);

    Platform* climb1 = new Platform(2900, 550, 150, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    Platform* climb2 = new Platform(3050, 500, 150, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    Platform* climb3 = new Platform(3200, 450, 150, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    Platform* climb4 = new Platform(3350, 400, 150, 20, Platform::PlatformType::Passthrough, Qt::yellow);

    scene->addItem(climb1);
    scene->addItem(climb2);
    scene->addItem(climb3);
    scene->addItem(climb4);
    platforms.append(climb1);
    platforms.append(climb2);
    platforms.append(climb3);
    platforms.append(climb4);

    Enemy* enemy = new Enemy();
    enemy->setPos(5500, 550);
    scene->addItem(enemy);
    enemies.append(enemy);

    if (goal) {
        scene->removeItem(goal);
        delete goal;
        goal = nullptr;
    }

    int flagWidth = 10;
    int flagHeight = 200;
    goal = new QGraphicsRectItem(0, 0, flagWidth, flagHeight);
    goal->setBrush(Qt::blue);
    goal->setPen(Qt::NoPen);


    int x = 5800;
    int y = 700 - flagHeight;
    goal->setPos(x, y);
    scene->addItem(goal);
}

void Level::setupLevel2() {
    width = 8000;  // Increased level width for complexity
    height = 1000; // Increased height for climbing challenges

    scene->setSceneRect(0, 0, width, height);

    // Create player at the starting position
    player = new Player();
    player->setPos(100, 800);  // Flat starting area
    scene->addItem(player);
    player->setFocus();

    // Starting flat ground platform (solid)
    Platform* floor = new Platform(0, 900, 1500, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(floor);
    platforms.append(floor);

    // Stationary enemy near the start
    Enemy* startEnemy = new Enemy();
    startEnemy->setPos(1300, 850);  // Positioned on the starting flat ground
    scene->addItem(startEnemy);
    enemies.append(startEnemy);

    // First challenge: gap with moving enemy on the other side
    Platform* gapLanding = new Platform(2000, 900, 800, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(gapLanding);
    platforms.append(gapLanding);

    Enemy* gapEnemy = new Enemy();
    gapEnemy->setPos(2200, 850);  // Moving back and forth on the landing platform
    scene->addItem(gapEnemy);
    enemies.append(gapEnemy);

    // Climbing section: stacked pass-through platforms with enemies
    Platform* climb1 = new Platform(2700, 750, 200, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    Platform* climb2 = new Platform(2900, 650, 200, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    Platform* climb3 = new Platform(3100, 550, 200, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    Platform* climb4 = new Platform(3300, 450, 200, 20, Platform::PlatformType::Passthrough, Qt::yellow);

    scene->addItem(climb1);
    scene->addItem(climb2);
    scene->addItem(climb3);
    scene->addItem(climb4);
    platforms.append(climb1);
    platforms.append(climb2);
    platforms.append(climb3);
    platforms.append(climb4);

    // Enemies on climbing platforms
    Enemy* climbEnemy1 = new Enemy();
    climbEnemy1->setPos(2800, 700);  // Moving back and forth
    scene->addItem(climbEnemy1);
    enemies.append(climbEnemy1);

    Enemy* climbEnemy2 = new Enemy();
    climbEnemy2->setPos(3000, 600);  // Moving back and forth
    scene->addItem(climbEnemy2);
    enemies.append(climbEnemy2);

    // Mid-level plateau for recovery
    Platform* midPlateau = new Platform(3500, 400, 1000, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(midPlateau);
    platforms.append(midPlateau);

    // Attack recoil challenge: gap with stationary enemies
    Platform* recoilStart = new Platform(4700, 900, 300, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(recoilStart);
    platforms.append(recoilStart);

    for (int i = 0; i < 4; i++) {
        Enemy* recoilEnemy = new Enemy();
        recoilEnemy->setPos(5100 + i * 200, 900);  // Evenly spaced stationary enemies
        scene->addItem(recoilEnemy);
        enemies.append(recoilEnemy);
    }

    Platform* recoilEnd = new Platform(6000, 900, 1000, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(recoilEnd);
    platforms.append(recoilEnd);

    // Final platform and goal area
    Platform* finalPlatform = new Platform(7000, 900, 1000, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(finalPlatform);
    platforms.append(finalPlatform);

    // Final enemy guarding the goal
    Enemy* finalEnemy = new Enemy();
    finalEnemy->setPos(7400, 850);
    scene->addItem(finalEnemy);
    enemies.append(finalEnemy);

    // Create the goal flag
    int flagWidth = 10;
    int flagHeight = 200;
    goal = new QGraphicsRectItem(0, 0, flagWidth, flagHeight);
    goal->setBrush(Qt::blue);
    goal->setPen(Qt::NoPen);
    goal->setPos(7800, 700);  // Positioned at the end of the final platform
    scene->addItem(goal);
}

void Level::setupLevel3() {
    width = 1000;  // Narrow vertical shaft
    height = 8000; // High vertical climb

    scene->setSceneRect(0, 0, width, height);

    // Create player at the starting position
    player = new Player();
    player->setPos(450, 7800);  // Bottom of the tower
    scene->addItem(player);
    player->setFocus();

    // Left and right solid walls to form a vertical shaft
    Platform* leftWall = new Platform(0, 0, 50, height, Platform::PlatformType::Solid, Qt::darkGreen);
    Platform* rightWall = new Platform(950, 0, 50, height, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(leftWall);
    scene->addItem(rightWall);
    platforms.append(leftWall);
    platforms.append(rightWall);

    // Starting area: flat ground with one platform to jump on
    Platform* groundFloor = new Platform(50, 7900, 900, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(groundFloor);
    platforms.append(groundFloor);

    Platform* startJump = new Platform(400, 7700, 200, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    scene->addItem(startJump);
    platforms.append(startJump);

    // First checkpoint area with gap
    Platform* checkpoint1 = new Platform(50, 7500, 350, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    Platform* gap1 = new Platform(400, 7500, 200, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    Platform* checkpoint1Right = new Platform(650, 7500, 300, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(checkpoint1);
    scene->addItem(gap1);
    scene->addItem(checkpoint1Right);
    platforms.append(checkpoint1);
    platforms.append(gap1);
    platforms.append(checkpoint1Right);

    // First climbing area: alternating platforms (already correct)
    int yPosition = 7300;
    for (int i = 0; i < 10; i++) {
        Platform::PlatformType type = (i % 2 == 0) ? Platform::PlatformType::Solid : Platform::PlatformType::Passthrough;
        Platform* climbPlatform = new Platform(400, yPosition, 200, 20, type, Qt::yellow);
        scene->addItem(climbPlatform);
        platforms.append(climbPlatform);

        if (i % 3 == 0) {
            Enemy* climbEnemy = new Enemy();
            climbEnemy->setPos(450, yPosition - 30);  // Positioned slightly above the platform
            scene->addItem(climbEnemy);
            enemies.append(climbEnemy);
        }
        yPosition -= 300;
    }

    // Second checkpoint area with gap
    Platform* checkpoint2 = new Platform(50, 4200, 350, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    Platform* gap2 = new Platform(400, 4200, 200, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    Platform* checkpoint2Right = new Platform(650, 4200, 300, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(checkpoint2);
    scene->addItem(gap2);
    scene->addItem(checkpoint2Right);
    platforms.append(checkpoint2);
    platforms.append(gap2);
    platforms.append(checkpoint2Right);

    // Improved second climbing area: alternating left and right with closer spacing
    yPosition = 4000;
    int xLeft = 300;
    int xRight = 500;
    for (int i = 0; i < 8; i++) {
        Platform* solidPlatform = new Platform((i % 2 == 0) ? xLeft : xRight, yPosition, 100, 20, Platform::PlatformType::Solid, Qt::darkGreen);
        Platform* passPlatform = new Platform((i % 2 == 0) ? xRight : xLeft, yPosition, 100, 20, Platform::PlatformType::Passthrough, Qt::yellow);
        scene->addItem(solidPlatform);
        scene->addItem(passPlatform);
        platforms.append(solidPlatform);
        platforms.append(passPlatform);

        if (i % 2 == 0) {
            Enemy* climbEnemy = new Enemy();
            climbEnemy->setPos((i % 2 == 0) ? xRight + 20 : xLeft + 20, yPosition - 30);
            scene->addItem(climbEnemy);
            enemies.append(climbEnemy);
        }
        yPosition -= 200;  // Reduced vertical gap for closer spacing
    }

    // Final checkpoint near the goal with gap
    Platform* checkpoint3 = new Platform(50, 1000, 350, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    Platform* gap3 = new Platform(400, 1000, 200, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    Platform* checkpoint3Right = new Platform(650, 1000, 300, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(checkpoint3);
    scene->addItem(gap3);
    scene->addItem(checkpoint3Right);
    platforms.append(checkpoint3);
    platforms.append(gap3);
    platforms.append(checkpoint3Right);

    // Final platform and goal area at the top of the tower
    Platform* goalPlatform = new Platform(400, 800, 200, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(goalPlatform);
    platforms.append(goalPlatform);

    // Final enemy guarding the goal
    Enemy* finalEnemy = new Enemy();
    finalEnemy->setPos(450, 750);
    scene->addItem(finalEnemy);
    enemies.append(finalEnemy);

    // Create the goal flag
    int flagWidth = 10;
    int flagHeight = 200;
    goal = new QGraphicsRectItem(0, 0, flagWidth, flagHeight);
    goal->setBrush(Qt::blue);
    goal->setPen(Qt::NoPen);
    goal->setPos(490, 600);  // Positioned at the top of the tower
    scene->addItem(goal);
}

void Level::setupLevel4() {
    width = 1000;
    height = 1000;
    scene->setSceneRect(0, 0, width, height);

    player = new Player();
    player->setPos(100, 400);
    scene->addItem(player);
    player->setFocus();

    // TODO: Add platforms, obstacles, etc.


    if (goal) {
        scene->removeItem(goal);
        delete goal;
        goal = nullptr;
    }

    // — Create a new flag
    int flagWidth  = 10;
    int flagHeight = 200;
    goal = new QGraphicsRectItem(0, 0, flagWidth, flagHeight);
    goal->setBrush(Qt::blue);
    goal->setPen(Qt::NoPen);

    // — Position it at the far right, just above the floor (y = 500)
    int offsetFromRight= 3000;
    int x = width - flagWidth - offsetFromRight;
    int y = 500 - flagHeight;   // use your floor’s Y
    goal->setPos(x, y);

    // — Add to scene
    scene->addItem(goal);
}


void Level::setupLevel5() {
    width = 1000;
    height = 1000;
    scene->setSceneRect(0, 0, width, height);

    player = new Player();
    player->setPos(100, 400);
    scene->addItem(player);
    player->setFocus();

    // TODO: Add platforms, obstacles, etc.

    if (goal) {
        scene->removeItem(goal);
        delete goal;
        goal = nullptr;
    }

    // — Create a new flag
    int flagWidth  = 10;
    int flagHeight = 200;
    goal = new QGraphicsRectItem(0, 0, flagWidth, flagHeight);
    goal->setBrush(Qt::blue);
    goal->setPen(Qt::NoPen);

    // — Position it at the far right, just above the floor (y = 500)
    int offsetFromRight= 3000;
    int x = width - flagWidth - offsetFromRight;
    int y = 500 - flagHeight;   // use your floor’s Y
    goal->setPos(x, y);

    // — Add to scene
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

    switch (level_type) {
    case LEVEL_1:
        level_type = LEVEL_2;
        break;
    case LEVEL_2:
        level_type = LEVEL_3;
        break;
    case LEVEL_3:
        level_type = LEVEL_4;
        break;
    case LEVEL_4:
        level_type = LEVEL_5;
        break;
    case LEVEL_5:
        return;
    }

    // 3) Rebuild the new level
    createLevel();
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

void Level::followPlayer(QGraphicsView* view) {
    if (!view || !player) return;

    // Dimensions of the viewport
    int viewHeight = view->viewport()->height();
    int viewWidth = view->viewport()->width();

    // Player position
    QPointF playerPos = player->pos();

    // Clamp the center Y so that the view never scrolls below the scene
    double halfViewHeight = viewHeight / 2.0;
    double maxY = height - halfViewHeight;
    double minY = halfViewHeight;
    double centerY = std::clamp(playerPos.y(), minY, maxY);

    // Clamp the center X similarly if desired (optional)
    double halfViewWidth = viewWidth / 2.0;
    double maxX = width - halfViewWidth;
    double minX = halfViewWidth;
    double centerX = std::clamp(playerPos.x(), minX, maxX);

    view->centerOn(centerX, centerY);
}
void Level::checkFlagCollision() {
    if (!goal || !player) return;

    // If the player's bounding box overlaps the flag...
    if (player->collidesWithItem(goal)) {
        // Advance to the next level
        update_level();
    }
}
