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
    width = 5000;
    height = 600;

    scene->setSceneRect(0 , 0 , width , height);

    // Create player
    player = new Player();
    player->setPos(100, 400);
    scene->addItem(player);
    player->setFocus();

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

    Enemy* enemy = new Enemy();
    enemy->setPos(850, 250);  // Position just above platform3 (800, 300)
    scene->addItem(enemy);
    enemies.append(enemy);
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


void Level::setupLevel2() {

    width = 2500;
    height = 600;

    scene->setSceneRect(0, 0, width, height);

    // Level 2 - harder layout with more obstacles

    // Create player
    player = new Player();
    player->setPos(100, 400);
    scene->addItem(player);
    player->setFocus();

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

    // TODO: Create obstacles
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
    int offsetFromRight= 600;
    int x = width - flagWidth - offsetFromRight;
    int y = 500 - flagHeight;   // use your floor’s Y
    goal->setPos(x, y);

    // — Add to scene
    scene->addItem(goal);
}


void Level::setupLevel3() {
    width = 800;
    height = 5000;
    scene->setSceneRect(0, 0, width, height);

    player = new Player();
    player->setPos(100, 400);
    scene->addItem(player);
    player->setFocus();

    Platform* floor = new Platform(0, 550, width, 200, Platform::PlatformType::Solid, Qt::darkGreen);

    scene->addItem(floor);
    platforms.append(floor);

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
