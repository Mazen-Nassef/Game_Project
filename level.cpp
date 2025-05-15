#include "level.h"
#include "enemy.h"
#include "mainwindow.h"
#include <QGraphicsView>
#include <QBrush>
#include <QPen>
#include <QObject>
#include <QMessageBox>

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

    for (auto text : texts) {
        scene->removeItem(text);
        delete text;
    }
    texts.clear();

    // Clear obstacles
    for (auto obstacle : obstacles) {
        scene->removeItem(obstacle);
        delete obstacle;
    }
    obstacles.clear();
    /*
    // Clear enemies and remove them from the scene
    for (auto enemy : enemies) {
        if (scene->items().contains(enemy)) {
            scene->removeItem(enemy);
        }
        removeEnemy(enemy); // Ensure enemy is removed from the enemies list
        enemy->deleteLater();
    }
    enemies.clear();
    */


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
    connect(player, SIGNAL(gameOver()), this, SLOT(onPlayerGameOver()));
    player->setFocus();

    Platform* floor = new Platform(0, 700, 2000, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(floor);
    platforms.append(floor);

    QGraphicsTextItem* text1 = new QGraphicsTextItem("Hello, Player! Use the arrow keys to move!");
    text1->setPos(300, 400);  // Position on the scene
    text1->setDefaultTextColor(Qt::black);  // Text color
    text1->setFont(QFont("Arial", 16));  // Font and size
    scene->addItem(text1);  // Add the text to the scene
    texts.append(text1);

    Platform* raisedLedge = new Platform(1500, 600, 500, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(raisedLedge);
    platforms.append(raisedLedge);

    QGraphicsTextItem* text2 = new QGraphicsTextItem("Use spacebar to jump!");
    text2->setPos(1200, 300);  // Position on the scene
    text2->setDefaultTextColor(Qt::black);  // Text color
    text2->setFont(QFont("Arial", 16));  // Font and size
    scene->addItem(text2);  // Add the text to the scene
    texts.append(text2);

    Platform* landingPlatform = new Platform(2400, 700, 1100, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(landingPlatform);
    platforms.append(landingPlatform);

    QGraphicsTextItem* text3 = new QGraphicsTextItem("Use M to dash!");
    text3->setPos(2100, 500);  // Position on the scene
    text3->setDefaultTextColor(Qt::black);  // Text color
    text3->setFont(QFont("Arial", 16));  // Font and size
    scene->addItem(text3);  // Add the text to the scene
    texts.append(text3);

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

    QGraphicsTextItem* text4 = new QGraphicsTextItem("use N to attack!");
    text4->setPos(5500, 400);  // Position on the scene
    text4->setDefaultTextColor(Qt::black);  // Text color
    text4->setFont(QFont("Arial", 16));  // Font and size
    scene->addItem(text4);  // Add the text to the scene
    texts.append(text4);

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
    width = 1000;  // Narrow vertical shaft
    height = 8000; // High vertical climb

    scene->setSceneRect(0, 0, width, height);

    // Create player at the starting position
    player = new Player();
    player->setPos(450, 7800);  // Bottom of the tower
    scene->addItem(player);
    connect(player, SIGNAL(gameOver()), this, SLOT(onPlayerGameOver()));
    player->setFocus();

    QGraphicsTextItem* text5 = new QGraphicsTextItem("Now for the harder stuff");
    text5->setPos(400, 7650);  // Position on the scene
    text5->setDefaultTextColor(Qt::black);  // Text color
    text5->setFont(QFont("Arial", 16));  // Font and size
    scene->addItem(text5);  // Add the text to the scene
    texts.append(text5);

    QGraphicsTextItem* text6 = new QGraphicsTextItem("hold the down and right keys, then quickly press space, M, then space again");
    text6->setPos(200, 7700);  // Position on the scene
    text6->setDefaultTextColor(Qt::black);  // Text color
    text6->setFont(QFont("Arial", 16));  // Font and size
    scene->addItem(text6);  // Add the text to the scene
    texts.append(text6);

    QGraphicsTextItem* text7 = new QGraphicsTextItem("this will make you perform a super jump!");
    text7->setPos(400, 7750);  // Position on the scene
    text7->setDefaultTextColor(Qt::black);  // Text color
    text7->setFont(QFont("Arial", 16));  // Font and size
    scene->addItem(text7);  // Add the text to the scene
    texts.append(text7);

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

    // First checkpoint area with gap
    Platform* checkpoint1 = new Platform(50, 7600, 350, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    Platform* gap1 = new Platform(400, 7600, 250, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    Platform* checkpoint1Right = new Platform(650, 7600, 300, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(checkpoint1);
    scene->addItem(gap1);
    scene->addItem(checkpoint1Right);
    platforms.append(checkpoint1);
    platforms.append(gap1);
    platforms.append(checkpoint1Right);

    // First climbing area: alternating platforms (already correct)
    int yPosition = 7300;
    /*
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
    */
    Platform* climbPlatformSolid1 = new Platform(50, 7300, 350, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(climbPlatformSolid1);
    platforms.append(climbPlatformSolid1);
    Platform* climbPlatformSolid2 = new Platform(650, 7000, 300, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(climbPlatformSolid2);
    platforms.append(climbPlatformSolid2);
    Platform* climbPlatformSolid3 = new Platform(50, 6700, 350, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(climbPlatformSolid3);
    platforms.append(climbPlatformSolid3);
    Platform* climbPlatformSolid4 = new Platform(650, 6400, 300, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(climbPlatformSolid4);
    platforms.append(climbPlatformSolid4);

    Platform* checkpoint2 = new Platform(50, 6100, 350, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    Platform* gap2 = new Platform(400, 6100, 250, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    Platform* checkpoint2Right = new Platform(650, 6100, 300, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(checkpoint2);
    scene->addItem(gap2);
    scene->addItem(checkpoint2Right);
    platforms.append(checkpoint2);
    platforms.append(gap2);
    platforms.append(checkpoint2Right);

    QGraphicsTextItem* text8 = new QGraphicsTextItem("Stand next to the wall, hold down, and attack to bounce up!");
    text8->setPos(400, 6000);  // Position on the scene
    text8->setDefaultTextColor(Qt::black);  // Text color
    text8->setFont(QFont("Arial", 16));  // Font and size
    scene->addItem(text8);  // Add the text to the scene
    texts.append(text8);

    // Second checkpoint area with gap
    Platform* checkpoint3 = new Platform(50, 4200, 850, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    Platform* gap3 = new Platform(900, 4200, 50, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    scene->addItem(checkpoint3);
    scene->addItem(gap3);
    platforms.append(checkpoint3);
    platforms.append(gap3);

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

    Platform* jump1 = new Platform(100, 2300, 100, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    scene->addItem(jump1);
    platforms.append(jump1);

    Platform* jump2 = new Platform(250, 2100, 100, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    scene->addItem(jump2);
    platforms.append(jump2);

    Platform* jump3 = new Platform(400, 1900, 100, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    scene->addItem(jump3);
    platforms.append(jump3);

    Platform* jump4 = new Platform(550, 1700, 100, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    scene->addItem(jump4);
    platforms.append(jump4);

    Platform* jump5 = new Platform(700, 1500, 100, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    scene->addItem(jump5);
    platforms.append(jump5);

    Platform* jump6 = new Platform(450, 1250, 300, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(jump6);
    platforms.append(jump6);

    // Final checkpoint near the goal with gap
    Platform* checkpoint4 = new Platform(50, 1000, 350, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    Platform* gap4 = new Platform(400, 1000, 250, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    Platform* checkpoint4Right = new Platform(650, 1000, 300, 20, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(checkpoint4);
    scene->addItem(gap4);
    scene->addItem(checkpoint4Right);
    platforms.append(checkpoint4);
    platforms.append(gap4);
    platforms.append(checkpoint4Right);

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

void Level::setupLevel3() {

    width = 8000;  // Increased level width for complexity
    height = 1000; // Increased height for climbing challenges

    scene->setSceneRect(0, 0, width, height);

    // Create player at the starting position
    player = new Player();
    player->setPos(100, 800);  // Flat starting area
    scene->addItem(player);
    connect(player, SIGNAL(gameOver()), this, SLOT(onPlayerGameOver()));
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

    for (int i = 0; i < 5; i++) {
        Enemy* recoilEnemy = new Enemy();
        recoilEnemy->setPos(5100 + i * 150, 900);  // Evenly spaced stationary enemies
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

void Level::setupLevel4() {
    width = 9000;
    height = 1000;

    scene->setSceneRect(0, 0, width, height);

    player = new Player();
    player->setPos(100, 800);  // Starting position
    scene->addItem(player);
    connect(player, SIGNAL(gameOver()), this, SLOT(onPlayerGameOver()));
    player->setFocus();

    // Flat ground at the start
    Platform* startGround = new Platform(0, 900, 1500, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(startGround);
    platforms.append(startGround);

    QGraphicsTextItem* text1 = new QGraphicsTextItem("Prepare for jumping challenges ahead!");
    text1->setPos(100, 700);
    text1->setDefaultTextColor(Qt::black);
    text1->setFont(QFont("Arial", 16));
    scene->addItem(text1);
    texts.append(text1);

    // Series of raised platforms with enemies
    int yPosition = 700;
    for (int i = 0; i < 5; i++) {
        Platform* platform = new Platform(1800 + i * 600, yPosition, 300, 20, Platform::PlatformType::Solid, Qt::darkGreen);
        scene->addItem(platform);
        platforms.append(platform);
        yPosition -= 50;

        Enemy* patrolEnemy = new Enemy();
        patrolEnemy->setPos(1950 + i * 600, yPosition - 30);
        scene->addItem(patrolEnemy);
        enemies.append(patrolEnemy);
    }

    // Narrow passage with multiple enemies
    Platform* passage = new Platform(4500, 900, 800, 100, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(passage);
    platforms.append(passage);

    for (int i = 0; i < 4; i++) {
        Enemy* ambushEnemy = new Enemy();
        ambushEnemy->setPos(4600 + i * 150, 850);
        scene->addItem(ambushEnemy);
        enemies.append(ambushEnemy);
    }

    QGraphicsTextItem* text2 = new QGraphicsTextItem("Be careful! Enemies ahead!");
    text2->setPos(4600, 700);
    text2->setDefaultTextColor(Qt::red);
    text2->setFont(QFont("Arial", 16));
    scene->addItem(text2);
    texts.append(text2);

    for (int i = 0; i < 4; i++) {
        Enemy* jumpenemy = new Enemy();
        jumpenemy->setPos(5500 + i * 110, 800 - i * 50);
        scene->addItem(jumpenemy);
        enemies.append(jumpenemy);
    }


    // Final climb to the goal
    Platform* finalClimb1 = new Platform(6000, 800, 200, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    Platform* finalClimb2 = new Platform(6300, 700, 200, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    Platform* finalClimb3 = new Platform(6600, 600, 200, 20, Platform::PlatformType::Passthrough, Qt::yellow);
    scene->addItem(finalClimb1);
    scene->addItem(finalClimb2);
    scene->addItem(finalClimb3);
    platforms.append(finalClimb1);
    platforms.append(finalClimb2);
    platforms.append(finalClimb3);

    Platform* finalFloor = new Platform(7000, 600, 2000, 800, Platform::PlatformType::Solid, Qt::darkGreen);
    scene->addItem(finalFloor);
    platforms.append(finalFloor);

    // Goal flag at the end
    goal = new QGraphicsRectItem(0, 0, 10, 200);
    goal->setBrush(Qt::blue);
    goal->setPen(Qt::NoPen);
    goal->setPos(8800, 400);
    scene->addItem(goal);
}

void Level::setupLevel5() {
    width = 1000;
    height = 1000;
    scene->setSceneRect(0, 0, width, height);

    player = new Player();
    player->setPos(100, 400);
    scene->addItem(player);
    connect(player, SIGNAL(gameOver()), this, SLOT(onPlayerGameOver()));
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


void Level::onPlayerGameOver() {
    if (gameOverHandled) return; // Already handled once
    gameOverHandled = true;

    QMessageBox::information(nullptr, "Game Over", "You Lose!");
    if (mainWindow)
        mainWindow->close();
}
