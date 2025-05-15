#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include "enemy.h"
#include "obstacle.h"
#include "platform.h"
#include "player.h"
#include <QMainWindow>

class MainWindow;
class Level : public QObject {
    Q_OBJECT
public:
    enum LevelType {
        LEVEL_1,
        LEVEL_2,
        LEVEL_3,
        LEVEL_4,
        LEVEL_5
    };


private:
    MainWindow* mainWindow;
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
    QList<Enemy*> enemies;
    QGraphicsRectItem* goal;
    Player* player;
    QList<QGraphicsTextItem*> texts;

    // Clear level elements
    void clearLevel();

    // Level setup methods
    void setupLevel1();
    void setupLevel2();
    void setupLevel3();
    void setupLevel4();
    void setupLevel5();

public:
    Level(QGraphicsScene* scene = nullptr);
    ~Level();

    // Configure the level
    void setLevelType(LevelType type);
    void setScene(QGraphicsScene* scene);

    void removeEnemy(Enemy* enemy);

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
    void followPlayer(QGraphicsView* view);
    void checkFlagCollision();
    // Get the player pointer
    Player* getPlayer() const { return player; }
    bool gameOverHandled = false;
public slots:
    void onPlayerGameOver();
};

#endif // LEVEL_H
