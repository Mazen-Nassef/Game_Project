#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QObject>
#include "ScoreSystem.h"
#include "player.h"
#include "enemy.h"
Enemy::Enemy(QGraphicsScene *scene, ObstacleType type, ScoreSystem *newscore, int speed, int h, int start, int end)
    : scene(scene), type(type), speed(speed), setscore(newscore), height(h), starting(start), ending(end){

    setPixmap(QPixmap("C:Users/LOQ/Desktop/Game_Project/goomba_super_mario.webp"));
    setScale(0.05);
    scene->addItem(this);
    setPos(starting, height);
    scene->update();

    if (type == Moving) {
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &Enemy::move);
        timer->start(50);
    }
    scene->update();
}

void Enemy::move() {
    if (type == Moving) {
        setPos(x() - speed, y());

        // for (QGraphicsItem *item : collidingItems()) {
        //     if (item->type() == QGraphicsPixmapItem::Type) {
        //         setscore->decrementscore();
        //         scene->removeItem(this);
        //         delete this;
        //         return;
        //     }
        // }


        if (x() <= ending || x() >= starting) {
            //this->setPos(starting, height);
            // scene->removeItem(this);
            // delete this;
            speed -= 2*speed;
        }
    }
}

bool Enemy::checkCollisionWithPlayer(Player *mainplayer) {
    if (collidesWithItem(mainplayer)) {

        return true;
    }
    return false;
}
