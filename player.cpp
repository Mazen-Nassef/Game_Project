#include <QBrush>
#include <QList>
#include "player.h"
#include "qgraphicsscene.h"


Player::Player()
    : health(3), speed(1), maxSpeed(5), friction(0.5), gravity(1), dashSpeed(10), dashDuration(150), length(50), width(30), jumpHeight(15),
    yVelocity(0), xVelocity(0), canDash(true), isOnGround(false), isOnPlatform(false), currentPlatform(nullptr), floorY(500), gravityTimer(new QTimer(this))
    // player stats, we can tweek until it feels right
{
    setRect(0, 0, width, length);
    setBrush(QBrush(Qt::red));
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
    // creates player and makes it red and the focus

    connect(gravityTimer, &QTimer::timeout, this, &Player::applyGravity);
    gravityTimer->start(16);
    // updates every 16ms, ~60fps, can change if required
}
void Player::keyPressEvent(QKeyEvent *event) {
    keysHeld.insert(event->key());

    switch (event->key()) {
    case Qt::Key_Left:
        moveBackward();
        break;
    case Qt::Key_Right:
        moveForward();
        break;
    case Qt::Key_Up:
        break;
    case Qt::Key_Space:
        // Immediately try to jump (more responsive than waiting for gravity update)
        if (isOnGround || isOnPlatform) {
            jump();
        }
        break;
    case Qt::Key_Down:
        // Handle dropping through platforms in the physics update
        break;
    case Qt::Key_M:
        performDash();
        break;
    }
    // this used to be how movement was handled but i started using keysHeld instead to make it smoother
    // some of it is probably deprecated, but i'm scared to touch this and break something, we can figure it out later
}

void Player::keyReleaseEvent(QKeyEvent *event) {
    keysHeld.remove(event->key());
} //this part makes key releases register in the keysHeld

void Player::moveForward() {
    xVelocity += speed;
} //holding forward increases players velocity by the magnitute of the speed value (positive for right)

void Player::moveBackward() {
    xVelocity -= speed;
} //holding back decreases players velocity by the magnitute of the speed value (negative for left)

void Player::jump() {
    // Simple jump implementation - just set upward velocity
    yVelocity = -jumpHeight;
    isOnGround = false;
    isOnPlatform = false;
    currentPlatform = nullptr;
}

#include "platform.h" // Add the include at the top of the file

#include "obstacle.h"
#include "staticobstacle.h"

bool Player::checkPlatformCollisions() {
    if (!scene()) {
        return false;
    }
    
    // Get all colliding items
    QList<QGraphicsItem*> collidingItems = scene()->collidingItems(this);
    
    // Reset platform status by default - we'll set it if we find we're on a platform
    isOnPlatform = false;
    currentPlatform = nullptr;
    bool isOnObstacle = false;
    
    // First check: Are we directly landing on a platform from above?
    qreal feetY = y() + rect().height();
    qreal smallGap = 5.0; // Small gap to detect "about to land" or "just left" platform
    
    for (QGraphicsItem* item : collidingItems) {
        // Check for platforms first
        Platform* platform = dynamic_cast<Platform*>(item);
        if (platform) {
            QRectF platformRect = platform->mapToScene(platform->rect()).boundingRect();
            
            // Check if player is directly above platform
            if (feetY >= platformRect.top() - smallGap && feetY <= platformRect.top() + smallGap) {
                // Player is directly on top of platform
                
                // For passthrough platforms, allow dropping if down key is pressed
                if (platform->isPassthrough() && keysHeld.contains(Qt::Key_Down)) {
                    // Allow player to drop through
                    continue;
                }
                
                // Otherwise, place player on top of platform and stop falling
                setY(platformRect.top() - rect().height());
                yVelocity = 0;
                isOnPlatform = true;
                currentPlatform = platform;
            }
            
            // Check for side collisions with solid platforms
            if (platform->isSolid()) {
                QRectF playerRect = mapToScene(rect()).boundingRect();
                
                // If player collides with platform sides
                if (playerRect.right() > platformRect.left() && playerRect.left() < platformRect.right() &&
                    playerRect.bottom() > platformRect.top() + smallGap && playerRect.top() < platformRect.bottom()) {
                    
                    // Determine if collision is from left or right
                    if (playerRect.right() - platformRect.left() < 10) {
                        // Player hit platform from the left
                        setX(platformRect.left() - rect().width());
                        xVelocity = 0;
                    }
                    else if (platformRect.right() - playerRect.left() < 10) {
                        // Player hit platform from the right
                        setX(platformRect.right());
                        xVelocity = 0;
                    }
                    else if (yVelocity < 0 && playerRect.top() < platformRect.bottom() && 
                             playerRect.top() > platformRect.top()) {
                        // Player is hitting platform ceiling
                        setY(platformRect.bottom());
                        yVelocity = 0;
                    }
                }
            }
        }
        
        // Now check for obstacles
        Obstacle* obstacle = dynamic_cast<Obstacle*>(item);
        if (obstacle) {
            QRectF obstacleRect = obstacle->mapToScene(obstacle->rect()).boundingRect();
            QRectF playerRect = mapToScene(rect()).boundingRect();
            
            // Check if player is directly above obstacle (standing on it)
            if (feetY >= obstacleRect.top() - smallGap && feetY <= obstacleRect.top() + smallGap) {
                // Place player on top of obstacle and stop falling
                setY(obstacleRect.top() - rect().height());
                yVelocity = 0;
                isOnObstacle = true;
                
                // Check if this is a bouncy obstacle
                StaticObstacle* staticObstacle = dynamic_cast<StaticObstacle*>(obstacle);
                if (staticObstacle && staticObstacle->getIsBouncy()) {
                    // Apply bounce - negative velocity with bounce strength
                    yVelocity = -jumpHeight * staticObstacle->getBounceStrength();
                    isOnObstacle = false; // We're bouncing, not standing
                }
                
                // Trigger the obstacle's collision response
                obstacle->collideWithPlayer(this);
            }
            // Check for side collisions with obstacles
            else if (playerRect.right() > obstacleRect.left() && 
                     playerRect.left() < obstacleRect.right() &&
                     playerRect.bottom() > obstacleRect.top() + smallGap && 
                     playerRect.top() < obstacleRect.bottom()) {
                
                // Determine if collision is from left or right
                if (playerRect.right() - obstacleRect.left() < 10) {
                    // Player hit obstacle from the left
                    setX(obstacleRect.left() - rect().width());
                    xVelocity = 0;
                    
                    // Trigger the obstacle's collision response
                    obstacle->collideWithPlayer(this);
                }
                else if (obstacleRect.right() - playerRect.left() < 10) {
                    // Player hit obstacle from the right
                    setX(obstacleRect.right());
                    xVelocity = 0;
                    
                    // Trigger the obstacle's collision response
                    obstacle->collideWithPlayer(this);
                }
                else if (yVelocity < 0 && playerRect.top() < obstacleRect.bottom() && 
                         playerRect.top() > obstacleRect.top()) {
                    // Player is hitting obstacle from below
                    setY(obstacleRect.bottom());
                    yVelocity = 0;
                    
                    // Trigger the obstacle's collision response
                    obstacle->collideWithPlayer(this);
                }
            }
        }
    }
    
    return isOnPlatform || isOnObstacle;
}

bool Player::canDropThroughPlatform() {
    // Can drop through if pressing down
    return keysHeld.contains(Qt::Key_Down);
}

void Player::performDash()
// I decided to add a dash because we need the bonus and because its more fun this way. this was probably the hardest part
{
    if (isDashing || !canDash) return;
    //prevents infinite dashing

    float dirX = 0, dirY = 0;
    if (keysHeld.contains(Qt::Key_Left))  dirX -= 1;
    if (keysHeld.contains(Qt::Key_Right)) dirX += 1;
    if (keysHeld.contains(Qt::Key_Up))    dirY -= 1;
    if (keysHeld.contains(Qt::Key_Down))  dirY += 1;
    // vector (math vector not DS vector) to determine dash direction

    if (dirX == 0 && dirY == 0)
        return;
    // nothing happens if the dash direction is 0


    float length = std::sqrt(dirX * dirX + dirY * dirY);
    dirX /= length;
    dirY /= length;
    // vector equation to get the unit circle normalized vector, I'm assuming you took calculus 2, either way don't worry about this

    dashX = dirX * dashSpeed;
    dashY = dirY * dashSpeed;
    // dashes in the direction held with the dash speed stat

    isDashing = true;
    canDash = false;
    // updates the flags

    setBrush(QBrush(Qt::blue));
    // makes the player blue, basic visual indicator for when the player can't dash

    yVelocity = 0;

    if (!dashTimer) dashTimer = new QTimer(this);
    dashTimer->setSingleShot(true);
    connect(dashTimer, &QTimer::timeout, this, [this]() {
        dashX = 0;
        dashY = 0;
        isDashing = false;
    });
    dashTimer->start(dashDuration);
    // asked chatgpt to make this part for me, since i don't understand QT timer logic
}

void Player::applyGravity()
// i turned this more into "apply physics" than "apply gravity", but i can't be bothered to change the name
// the important thing is that it handles all movement now, not just vertical movement
{
    // Store previous states
    bool wasOnGround = isOnGround;
    bool wasOnPlatform = isOnPlatform;
    
    // Handle jumping with better key detection
    // Using Key_Space for jumping
    if ((keysHeld.contains(Qt::Key_Space)) &&
        (isOnGround || isOnPlatform)) {
        // Jump only if we're on a solid surface
        jump();
    }
    
    // Apply gravity if not dashing
    if (!isDashing) {
        yVelocity += gravity;
    }

    // Handle left/right movement
    if (keysHeld.contains(Qt::Key_Left)) {
        moveBackward();
    }
    if (keysHeld.contains(Qt::Key_Right)) {
        moveForward();
    }

    // Update position
    setX(x() + xVelocity + dashX);
    setY(y() + yVelocity + dashY);
    
    // Handle floor collision first (simpler)
    isOnGround = false;
    if (y() >= floorY) {
        setY(floorY);
        yVelocity = 0;
        isOnGround = true;
    }
    
    // Only check platform collisions if we're not on ground
    bool onSolidSurface = isOnGround;
    if (!isOnGround) {
        onSolidSurface = checkPlatformCollisions();
    }
    
    // Reset dash ability when landing on any solid surface
    bool justLanded = (isOnGround && !wasOnGround) || (isOnPlatform && !wasOnPlatform);
    if (onSolidSurface && !canDash && !isDashing) {
        canDash = true;
        setBrush(QBrush(Qt::red));
    }
    
    // Stop dashing when landing
    if (justLanded && isDashing) {
        isDashing = false;
        dashX = 0;
        dashY = 0;
        
        if (dashTimer && dashTimer->isActive()) {
            dashTimer->stop();
        }
    }

    // Apply friction if not dashing
    if (!isDashing) {
        float currentFriction = friction;

        if (std::abs(xVelocity) > maxSpeed) {
            currentFriction += (std::abs(xVelocity) - maxSpeed) * 0.2;
        }

        if (xVelocity > 0) {
            xVelocity -= currentFriction;
            if (xVelocity < 0) xVelocity = 0;
        } else if (xVelocity < 0) {
            xVelocity += currentFriction;
            if (xVelocity > 0) xVelocity = 0;
        }
    }
    
    // Debug info - can be removed after fixing
    // qDebug() << "isOnGround:" << isOnGround << "isOnPlatform:" << isOnPlatform
    //          << "canJump:" << (isOnGround || isOnPlatform) << "yVel:" << yVelocity;
}
    
void Player::takeDamage()
{
    // Call the Health object's takeDamage method
    health.takeDamage();
    
    // Visual indication of damage
    setBrush(QBrush(Qt::yellow));
    
    // Reset to normal color after a short delay
    QTimer::singleShot(300, this, [this]() {
        if (!isDashing) {
            setBrush(QBrush(Qt::red));
        } else {
            setBrush(QBrush(Qt::blue));
        }
    });
    
    // Check if player is still alive
    if (health.get() <= 0) {
        // Player has died
        reset();
    }
}
    
void Player::reset()
{
    // Reset player position
    setPos(0, 0);
    
    // Reset velocities
    xVelocity = 0;
    yVelocity = 0;
    
    // Reset dash state
    isDashing = false;
    canDash = true;
    dashX = 0;
    dashY = 0;
    
    // Reset platform state
    isOnGround = false;
    isOnPlatform = false;
    currentPlatform = nullptr;
    
    // Reset color
    setBrush(QBrush(Qt::red));
    
    // Reset health
    health.reset();
}
