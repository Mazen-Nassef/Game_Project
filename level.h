#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include "player.h"

class Level {
private:
    int current_level;
    double frequency_multiplier;
    double velocity_multiplier;
    double damage_multiplier;

    void set_frequencyM();
    void set_velocityM();
    void set_damageM();

    QList<QGraphicsRectItem*> platforms;
    QGraphicsPixmapItem* goal;

public:
    Level();
    int get_level() const;
    double get_frequencyM() const;
    double get_velocityM() const;
    double get_damageM() const;
    void update_level();

    void setupLevel(QGraphicsScene* scene);
    void followPlayer(QGraphicsView* view, Player* player);
};

#endif // LEVEL_H
