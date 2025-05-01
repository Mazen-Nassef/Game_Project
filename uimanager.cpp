#include "uimanager.h"
#include "mainwindow.h"
#include "player.h"
#include "level.h"

UIManager::UIManager(MainWindow* parent)
    : QObject(parent), score(0), gameTime(0.0), displayedHealth(3), parentWindow(parent)
{
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &UIManager::onGameTimerTick);
}

UIManager::~UIManager() {
    if (gameTimer) {
        gameTimer->stop();
    }
}

void UIManager::setupUI(QWidget* parent) {
    // Create UI labels
    scoreLabel = new QLabel(parent);
    healthLabel = new QLabel(parent);
    timeLabel = new QLabel(parent);

    // Set labels with right alignment and make them stand out against any background
    QString styleSheet = "color: black; font: bold 16px; background-color: rgba(255, 255, 255, 150); padding: 2px 5px; border-radius: 3px;";
    scoreLabel->setStyleSheet(styleSheet);
    healthLabel->setStyleSheet(styleSheet);
    timeLabel->setStyleSheet(styleSheet);
    
    // Align text to the right within each label
    scoreLabel->setAlignment(Qt::AlignRight);
    healthLabel->setAlignment(Qt::AlignRight);
    timeLabel->setAlignment(Qt::AlignRight);
    
    // Set initial position
    scoreLabel->move(10, 10);
    healthLabel->move(10, 40);
    timeLabel->move(10, 70);

    scoreLabel->raise();
    healthLabel->raise();
    timeLabel->raise();

    scoreLabel->show();
    healthLabel->show();
    timeLabel->show();

    // Initialize UI values
    updateScore(score);
    updateHealth(displayedHealth);
    updateTime(gameTime);

    // Create level switch buttons
    level1Button = new QPushButton("Level 1", parent);
    level2Button = new QPushButton("Level 2", parent);
    level3Button = new QPushButton("Level 3", parent);
    level4Button = new QPushButton("Level 4", parent);
    level5Button = new QPushButton("Level 5", parent);
    
    connect(level1Button, &QPushButton::clicked, this, [this]() {
        emit levelSwitchRequested(Level::LEVEL_1);
    });
    
    connect(level2Button, &QPushButton::clicked, this, [this]() {
        emit levelSwitchRequested(Level::LEVEL_2);
    });

    connect(level3Button, &QPushButton::clicked, this, [this]() {
        emit levelSwitchRequested(Level::LEVEL_3);
    });

    connect(level4Button, &QPushButton::clicked, this, [this]() {
        emit levelSwitchRequested(Level::LEVEL_4);
    });
    
    connect(level5Button, &QPushButton::clicked, this, [this]() {
        emit levelSwitchRequested(Level::LEVEL_5);
    });

    // Position the buttons
    level1Button->move(10, parent->height() - 40);
    level2Button->move(90, parent->height() - 40);
    level3Button->move(170, parent->height() - 40);
    level4Button->move(250, parent->height() - 40);
    level5Button->move(330, parent->height() - 40);
    
    level1Button->show();
    level2Button->show();
    level3Button->show();
    level4Button->show();
    level5Button->show();
}

void UIManager::positionLabels(QGraphicsView* view) {
    if (!view || !scoreLabel || !healthLabel || !timeLabel)
        return;
        
    // Position UI elements in the top right corner of the viewport
    int rightMargin = 20; // Distance from right edge
    int topMargin = 10;   // Distance from top edge
    int labelSpacing = 30; // Vertical space between labels
    
    // Calculate right-aligned positions based on viewport width
    int viewportWidth = view->viewport()->width();
    
    // Position labels in top-right corner with right alignment
    scoreLabel->move(viewportWidth - scoreLabel->width() - rightMargin, topMargin);
    healthLabel->move(viewportWidth - healthLabel->width() - rightMargin, topMargin + labelSpacing);
    timeLabel->move(viewportWidth - timeLabel->width() - rightMargin, topMargin + labelSpacing * 2);
    
    // Make sure UI is always on top
    scoreLabel->raise();
    healthLabel->raise();
    timeLabel->raise();
}

void UIManager::setScoreLabel(QLabel* label) {
    scoreLabel = label;
    updateScore(score);
}

void UIManager::setHealthLabel(QLabel* label) {
    healthLabel = label;
    updateHealth(displayedHealth);
}

void UIManager::setTimeLabel(QLabel* label) {
    timeLabel = label;
    updateTime(gameTime);
}

void UIManager::setMessageLabel(QLabel* label) {
    messageLabel = label;
    if (messageLabel && !currentMessage.isEmpty()) {
        messageLabel->setText(currentMessage);
    }
}

void UIManager::updateHealth(int health) {
    displayedHealth = health;
    if (healthLabel) {
        healthLabel->setText("Health: " + QString::number(displayedHealth));
    }
}

void UIManager::updateScore(int newScore) {
    score = newScore;
    if (scoreLabel) {
        scoreLabel->setText("Score: " + QString::number(score));
    }
}

void UIManager::updateTime(double time) {
    gameTime = time;
    if (timeLabel) {
        timeLabel->setText("Time: " + QString::number(gameTime, 'f', 2) + "s");
    }
}

void UIManager::showMessage(const QString& message) {
    currentMessage = message;
    if (messageLabel) {
        messageLabel->setText(currentMessage);
    }
}

void UIManager::resetUI() {
    score = 0;
    gameTime = 0.0;
    updateScore(score);
    updateTime(gameTime);
}

void UIManager::startGameTimer() {
    if (gameTimer) {
        gameTimer->start(1000 / 60); // 60 FPS
    }
}

void UIManager::stopGameTimer() {
    if (gameTimer) {
        gameTimer->stop();
    }
}

void UIManager::connectPlayerSignals(Player* player) {
    if (player) {
        // Disconnect any existing connections to avoid duplicates
        disconnect(player, &Player::healthChanged, this, &UIManager::onPlayerHealthChanged);
        
        // Connect player's health signal to our handler
        connect(player, &Player::healthChanged, this, &UIManager::onPlayerHealthChanged);
        
        // Initialize with current health
        updateHealth(player->getHealth());
    }
}

void UIManager::onGameTimerTick() {
    // Update time by 1/60th of a second (assuming 60fps)
    gameTime += 1.0 / 60.0;
    updateTime(gameTime);
    
    // Update score
    score += 1;
    updateScore(score);
    emit scoreUpdated(score);
    
    // Request camera update
    emit cameraUpdateRequested();
}

void UIManager::onPlayerHealthChanged(int newHealth) {
    updateHealth(newHealth);
}

// Include the MOC generated file to ensure signals/slots work properly
#include "moc_uimanager.cpp"
