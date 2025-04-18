#include "level.h"
#include <QGraphicsView>

Level::Level() {
    current_level = 1;
    set_frequencyM();
    set_velocityM();
    set_damageM();
}

int Level::get_level() const {
    return current_level;
}

double Level::get_damageM() const {
    return damage_multiplier;
}

double Level::get_frequencyM() const {
    return frequency_multiplier;
}

double Level::get_velocityM() const {
    return velocity_multiplier;
}

void Level::update_level() {
    current_level++;
    set_frequencyM();
    set_velocityM();
    set_damageM();
}

void Level::set_frequencyM() {
    frequency_multiplier = 1.0 + 0.2 * (current_level - 1);
}

void Level::set_velocityM() {
    velocity_multiplier = 1.0 + 0.15 * (current_level - 1);
}

void Level::set_damageM() {
    damage_multiplier = 1.0 + 0.1 * (current_level - 1);
}

void Level::setupLevel(QGraphicsScene* scene) {
    // Clear old platforms
    for (auto p : platforms) {
        scene->removeItem(p);
        delete p;
    }
    platforms.clear();

    // Create some platforms
    QGraphicsRectItem* ground = new QGraphicsRectItem(0, 550, 1600, 50);
    ground->setBrush(Qt::darkGray);
    scene->addItem(ground);
    platforms.append(ground);

    QGraphicsRectItem* platform1 = new QGraphicsRectItem(200, 450, 100, 20);
    platform1->setBrush(Qt::gray);
    scene->addItem(platform1);
    platforms.append(platform1);

    QGraphicsRectItem* platform2 = new QGraphicsRectItem(500, 350, 100, 20);
    platform2->setBrush(Qt::gray);
    scene->addItem(platform2);
    platforms.append(platform2);

    // Add goal
    goal = new QGraphicsPixmapItem(QPixmap(":/images/flag.png").scaled(50, 50));
    goal->setPos(1500, 500);  // end of the level
    scene->addItem(goal);
}

void Level::followPlayer(QGraphicsView* view, Player* player) {
    int playerX = player->x();
    int centerX = view->width() / 2;
    view->centerOn(playerX, 300); // vertical center can be fixed
}
