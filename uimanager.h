#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include <QString>
#include <QLabel>
#include <QTimer>
#include <QWidget>
#include <QGraphicsView>
#include <QPushButton>

// Forward declarations
class MainWindow;
class Level;
class Player;

class UIManager : public QObject
{
    Q_OBJECT

private:
    int score;
    double gameTime;
    int displayedHealth;
    QString currentMessage;
    MainWindow* parentWindow;

    // UI Elements
    QLabel* scoreLabel = nullptr;
    QLabel* healthLabel = nullptr;
    QLabel* timeLabel = nullptr;
    QLabel* messageLabel = nullptr;
    QPushButton* level1Button = nullptr;
    QPushButton* level2Button = nullptr;
    QPushButton* level3Button = nullptr;
    QPushButton* level4Button = nullptr;
    QPushButton* level5Button = nullptr;

    // Game timer
    QTimer* gameTimer = nullptr;

public:
    UIManager(MainWindow* parent = nullptr);
    ~UIManager();

    // UI Setup
    void setupUI(QWidget* parent);
    void positionLabels(QGraphicsView* view);
    
    // Label accessors 
    void setScoreLabel(QLabel* label);
    void setHealthLabel(QLabel* label);
    void setTimeLabel(QLabel* label);
    void setMessageLabel(QLabel* label);

    // UI Update methods
    void updateScore(int score);
    void updateHealth(int health);
    void updateTime(double timeElapsed);
    void showMessage(const QString& message);
    void resetUI();
    
    // Timer management
    void startGameTimer();
    void stopGameTimer();
    
    // Connect player signals to UI
    void connectPlayerSignals(Player* player);
    
    // Get current UI values
    int getScore() const { return score; }
    double getGameTime() const { return gameTime; }

private slots:
    void onGameTimerTick();
    void onPlayerHealthChanged(int newHealth);

signals:
    void levelSwitchRequested(int levelType);
    void scoreUpdated(int newScore);
    void cameraUpdateRequested();
};

#endif // UIMANAGER_H
