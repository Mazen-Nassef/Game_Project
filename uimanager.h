#ifndef UIMANAGER_H
#define UIMANAGER_H
#include <QString>

class UIManager
{
private:
    int displayedScore ;
    double ElapsedTime ;
    int displayedHealth;
    QString currentMessage;

public:
    UIManager();

    void updateScore(int score);
    void updateTime(double timeRemaining);
    void updateHealth(int health);
    void showMessage(const QString& message);
};

#endif // UIMANAGER_H
