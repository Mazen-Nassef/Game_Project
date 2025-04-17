#include "uimanager.h"

UIManager::UIManager() : displayedScore(0) , ElapsedTime(0.0) , displayedHealth(100)
{}

void UIManager::updateHealth(int Health){
    displayedHealth = Health;
}

void UIManager::updateScore(int score){
    displayedScore=score;
}

void UIManager::updateTime(double time){
    ElapsedTime=time;
}

void UIManager::showMessage(const QString& message) {
    currentMessage = message;
}
