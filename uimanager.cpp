#include "uimanager.h"

UIManager::UIManager()
    : displayedScore(0), ElapsedTime(0.0), displayedHealth(100)
{}

void UIManager::setScoreLabel(QLabel* label) {
    scoreLabel = label;
}

void UIManager::setHealthLabel(QLabel* label) {
    healthLabel = label;
}

void UIManager::setTimeLabel(QLabel* label) {
    timeLabel = label;
}

void UIManager::setMessageLabel(QLabel* label) {
    messageLabel = label;
}

void UIManager::updateHealth(int health) {
    displayedHealth = health;
    if (healthLabel) {
        healthLabel->setText("Health: " + QString::number(displayedHealth));
    }
}

void UIManager::updateScore(int score) {
    displayedScore = score;
    if (scoreLabel) {
        scoreLabel->setText("Score: " + QString::number(displayedScore));
    }
}

void UIManager::updateTime(double time) {
    ElapsedTime = time;
    if (timeLabel) {
        timeLabel->setText("Time: " + QString::number(ElapsedTime, 'f', 2) + "s");
    }
}

void UIManager::showMessage(const QString& message) {
    currentMessage = message;
    if (messageLabel) {
        messageLabel->setText(currentMessage);
    }
}
