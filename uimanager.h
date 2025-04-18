#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QString>
#include <QLabel>

class UIManager
{
private:
    int displayedScore;
    double ElapsedTime;
    int displayedHealth;
    QString currentMessage;

    QLabel* scoreLabel = nullptr;
    QLabel* healthLabel = nullptr;
    QLabel* timeLabel = nullptr;
    QLabel* messageLabel = nullptr;

public:
    UIManager();

    void setScoreLabel(QLabel* label);
    void setHealthLabel(QLabel* label);
    void setTimeLabel(QLabel* label);
    void setMessageLabel(QLabel* label);

    void updateScore(int score);
    void updateHealth(int health);
    void updateTime(double timeRemaining);
    void showMessage(const QString& message);
};

#endif
