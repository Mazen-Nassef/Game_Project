#include <QBrush>
#include <QList>
#include "player.h"
#include "enemy.h"
#include "qgraphicsscene.h"
#include "platform.h"
#include "obstacle.h"


Player::Player()
    : health(3), speed(1), maxSpeed(5), friction(0.5), gravity(1), dashSpeed(10), dashDuration(150), length(50), width(30), jumpHeight(15),
    yVelocity(0), xVelocity(0), canDash(true), isOnGround(false), isOnPlatform(false), currentPlatform(nullptr), gravityTimer(new QTimer(this)),
    attackTimer(nullptr), attackCollisionTimer(nullptr), isAttacking(false), attackDuration(500), attackLaunchMagnitude(15), attackGraphic(nullptr), attackDistance(30),
    attackDirectionX(0), attackDirectionY(0)
    // player stats, we can tweek until it feels right
{
    setRect(0, 0, width, length);
    setBrush(QBrush(Qt::red));
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
    // creates player and makes it red and the focus for inputs

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

        break;
    case Qt::Key_Down:
        break;
    case Qt::Key_M:
        performDash();
        break;
    case Qt::Key_N:
        performAttack();
        break;
    }
    // this used to be how movement was handled but i started using keysHeld instead to make it smoother
    // some of it is probably deprecated, but i'm scared to touch this and break something, we can figure it out later
    // nevermind, not deprecated, my pc almost blew up when i tried removing them. never touching this again
}

void Player::keyReleaseEvent(QKeyEvent *event) {
    keysHeld.remove(event->key());
} // this part makes key releases register in the keysHeld

void Player::moveForward() {
    xVelocity += speed;
} // holding forward increases players velocity by the magnitute of the speed value (positive for right)

void Player::moveBackward() {
    xVelocity -= speed;
} // holding back decreases players velocity by the magnitute of the speed value (negative for left)

void Player::jump() {
    // Simple jump implementation - just set upward velocity
    // might be worth considering changing from a pure setter to an incrementation, but in most cases its the same
    yVelocity = -jumpHeight;
    isOnGround = false;
    isOnPlatform = false;
    currentPlatform = nullptr;
    // updates the flags for the gravity function
}



bool Player::checkPlatformCollisions() {
    if (!scene()) {
        return false;
    }
    // obviously, this is mostly useless unless something breaks
    
    QList<QGraphicsItem*> collidingItems = scene()->collidingItems(this);
    // full list of all collding items (thanks QT!)

    isOnPlatform = false;
    currentPlatform = nullptr;
    bool isOnObstacle = false;
    // default values
    
    qreal feetY = y() + rect().height();
    qreal smallGap = std::abs(yVelocity + dashY) + 1.0;
    // "smallGap" is just how far the player is expected to move in the next frame, plus a little extra for safety
    // will use this later on to stop clipping through stuff

    
    for (QGraphicsItem* item : collidingItems) {
        Platform* platform = dynamic_cast<Platform*>(item);
        // check for platforms
        if (platform)
        {
            QRectF platformRect = platform->mapToScene(platform->rect()).boundingRect();
            if (yVelocity > 0){
            if (feetY >= platformRect.top() - smallGap && feetY <= platformRect.top() + smallGap) {
                    // checks if player is about to fall on a platform
                
                if (platform->isPassthrough() && keysHeld.contains(Qt::Key_Down)) {
                    // check if the platform is passthrough and the player is holding down
                    continue;
                    // allow player to drop through
                }
                
                setY(platformRect.top() - rect().height());
                yVelocity = 0;
                isOnPlatform = true;
                currentPlatform = platform;
                // otherwise stops the player from falling
            }
        }
            if (platform->isSolid()) {
                QRectF playerRect = mapToScene(rect()).boundingRect();
                // checks for side collisions with solid platforms

                if (playerRect.right() > platformRect.left() && playerRect.left() < platformRect.right() &&
                    playerRect.bottom() > platformRect.top() + smallGap && playerRect.top() < platformRect.bottom()) {
                    // checks if player is about to collide with platform side
                    // just realized we might need to add a horizonral small gap function, but we're probably fine

                    if (playerRect.right() - platformRect.left() < 10) {
                        // checks what direction collision is coming from
                        setX(platformRect.left() - rect().width());
                        xVelocity = 0;
                        // the player stops
                    }
                    else if (platformRect.right() - playerRect.left() < 10) {
                        setX(platformRect.right());
                        xVelocity = 0;
                        // the player also stops but from the other direction now
                    }
                    else if (yVelocity < 0 && playerRect.top() < platformRect.bottom() &&
                             playerRect.top() > platformRect.top()) {
                        // checks if the player is hitting the platform ceiling
                        setY(platformRect.bottom());
                        yVelocity = 0;
                        // stop the player
                    }
                }
            }
        }
        
        Obstacle* obstacle = dynamic_cast<Obstacle*>(item);
        // we should probably consolidate obstacles and platforms, but that will take too much time, we'll just do the whole thing twice
        if (obstacle) {
            QRectF obstacleRect = obstacle->mapToScene(obstacle->rect()).boundingRect();
            QRectF playerRect = mapToScene(rect()).boundingRect();
            
            if (feetY >= obstacleRect.top() - smallGap && feetY <= obstacleRect.top() + smallGap) {
                // checks if player is about to fall on a platform - i mean obstacle
                setY(obstacleRect.top() - rect().height());
                yVelocity = 0;
                isOnObstacle = true;
                // no need to check for passthrough, directly stop the player

                Obstacle* Obstacle = dynamic_cast<class Obstacle*>(obstacle);
                if (Obstacle) {
                    if (obstacle->getIsStompable()) {
                        // stomp check
                        Obstacle->deleteObstacle();
                        // stomp done, obstacle gone
                    }
                    else if (Obstacle->getIsBouncy()) {
                        // bounce check
                        yVelocity = -jumpHeight * Obstacle->getBounceStrength();
                        // boing
                        isOnObstacle = false;
                        // we're bouncing, not standing
                    }
                }
                
                if (obstacle->getDamage() > 0) {
                    obstacle->collideWithPlayer(this);
                    // player damage time
                }
            }

            // this next part is all claude since i struggled to figure this out and we're out of time to have fun
            // start of claude code

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

            // end of claude code
        }
    }
    
    return isOnPlatform || isOnObstacle;

    //update the thing
}

bool Player::canDropThroughPlatform() {
    return keysHeld.contains(Qt::Key_Down);
    // can drop through if pressing down, simple enough
}

void Player::performDash()
// i decided to add a dash because we need the bonus and because its more fun this way. this was probably the hardest part
{
    if (isDashing || !canDash) return;
    //prevents infinite dashing

    float dirX = 0, dirY = 0;
    if (keysHeld.contains(Qt::Key_Left))  dirX -= 1;
    if (keysHeld.contains(Qt::Key_Right)) dirX += 1;
    if (keysHeld.contains(Qt::Key_Up))    dirY -= 1;
    if (keysHeld.contains(Qt::Key_Down))  dirY += 1;
    // vector (math vector not DS vector) to determine dash direction

    float length = std::sqrt(dirX * dirX + dirY * dirY);
    if (length == 0)
        return;
    // nothing happens if the dash direction is 0

    dirX /= length;
    dirY /= length;
    // vector equation to get the unit circle normalized vector, I'm assuming you took calc 2, either way don't worry about this

    xVelocity = dirX * dashSpeed;
    yVelocity = dirY * dashSpeed;
    // dashes in the direction held with the dash speed stat

    isDashing = true;
    canDash = false;
    // updates the flags

    setBrush(QBrush(Qt::blue));
    // makes the player blue, basic visual indicator for when the player can't dash

    // yVelocity = 0;

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
    //  bool wasOnGround = isOnGround;
    //  bool wasOnPlatform = isOnPlatform;
    //  // stores previous states

    if ((keysHeld.contains(Qt::Key_Space)) &&
        (isOnGround || isOnPlatform)) {
        jump();
    }
    // jumps only if we're on a solid surface
    // will probably need to update to include obstacles, depending on how we decide to use those

    if (!isDashing) {
        yVelocity += gravity;
    }
    // applies gravity unless dashing

    if (keysHeld.contains(Qt::Key_Left)) {
        moveBackward();
    }
    if (keysHeld.contains(Qt::Key_Right)) {
        moveForward();
    }
    // handles left/right movement

    setX(x() + xVelocity);
    setY(y() + yVelocity);
    // updates position

    // Update attack position if attacking
    if (isAttacking && attackGraphic) {
        updateAttackPosition();
    }

    isOnGround = false;

    bool onSolidSurface = isOnGround;
    if (!isOnGround) {
        onSolidSurface = checkPlatformCollisions();
    }
    // checks platform collisions only if we're not on ground

    //bool justLanded = (isOnGround && !wasOnGround) || (isOnPlatform && !wasOnPlatform);
    if (onSolidSurface && !canDash) {
        canDash = true;
        setBrush(QBrush(Qt::red));
        // resets the dash when landing on a solid surface
        // again, might need to update to include obstacles
    }

    //if (justLanded && isDashing) {
    //    isDashing = false;
    //    dashX = 0;
    //    dashY = 0;
    //    // stops dashing when landing
    //    if (dashTimer && dashTimer->isActive()) {
    //        dashTimer->stop();
    //    }
    //}

    // will need to update above function to make it more fun

    if (!isDashing) {
        float currentFriction = friction;
        // apply friction if not dashing

        if (std::abs(xVelocity) > maxSpeed) {
            currentFriction += (std::abs(xVelocity) - maxSpeed) * 0.2;
        }
        // increases friction if above max speed

        if (xVelocity > 0) {
            xVelocity -= currentFriction;
            if (xVelocity < 0) xVelocity = 0;
            // applies friction
        } else if (xVelocity < 0) {
            xVelocity += currentFriction;
            if (xVelocity > 0) xVelocity = 0;
            // applies friction the other way around
        }
    }
    // Get current position
    QPointF pos = this->pos();

    // Get scene boundaries (assumes Level sets scene rect correctly)
    QRectF bounds = scene()->sceneRect();

    // Clamp X
    if (pos.x() < bounds.left())
        pos.setX(bounds.left());
    else if (pos.x() + boundingRect().width() > bounds.right())
        pos.setX(bounds.right() - boundingRect().width());

    // Clamp Y
    if (pos.y() < bounds.top())
        pos.setY(bounds.top());
    else if (pos.y() + boundingRect().height() > bounds.bottom()) {
        pos.setY(bounds.bottom() - boundingRect().height());
        // If the player falls below the bottom boundary, call reset
        reset();
    }

    // Set clamped position
    setPos(pos);

}
    
void Player::takeDamage(int amount)
{
    // Call the Health object's takeDamage method with the specified amount
    health.takeDamage(amount);
    
    // Emit signal to update UI with new health value
    emit healthChanged(health.get());
    
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
    
void Player::performAttack()
{
    // Don't allow attacking if already attacking
    if (isAttacking) return;
    
    // Determine attack direction using the same logic as dash
    float dirX = 0, dirY = 0;
    if (keysHeld.contains(Qt::Key_Left))  dirX -= 1;
    if (keysHeld.contains(Qt::Key_Right)) dirX += 1;
    if (keysHeld.contains(Qt::Key_Up))    dirY -= 1;
    if (keysHeld.contains(Qt::Key_Down))  dirY += 1;
    
    // Default to forward attack if no direction is pressed
    if (dirX == 0 && dirY == 0) {
        // Check which way the player is facing based on last movement
        if (xVelocity > 0) dirX = 1;  // Facing right
        else if (xVelocity < 0) dirX = -1;  // Facing left
        else dirX = 1;  // Default to right if no horizontal velocity
    }
    
    // Normalize the direction vector
    float length = std::sqrt(dirX * dirX + dirY * dirY);
    dirX /= length;
    dirY /= length;
    
    // Save the attack direction for collision response
    attackDirectionX = dirX;
    attackDirectionY = dirY;
    
    // Create the crescent attack graphic
    attackGraphic = createAttackGraphic(dirX, dirY);
    if (scene()) {
        scene()->addItem(attackGraphic);
    }
    
    // Set attack state
    isAttacking = true;
    
    // Create attack duration timer if it doesn't exist
    if (!attackTimer) {
        attackTimer = new QTimer(this);
        attackTimer->setSingleShot(true);
        connect(attackTimer, &QTimer::timeout, this, &Player::removeAttack);
    }
    
    // Create collision check timer if it doesn't exist
    if (!attackCollisionTimer) {
        attackCollisionTimer = new QTimer(this);
        connect(attackCollisionTimer, &QTimer::timeout, this, &Player::checkAttackCollision);
    }
    
    // Start timers
    attackTimer->start(attackDuration);
    attackCollisionTimer->start(16); // Check collisions at ~60fps
}

void Player::removeAttack()
{
    // Clean up the attack graphic
    if (attackGraphic) {
        if (scene()) {
            scene()->removeItem(attackGraphic);
        }
        delete attackGraphic;
        attackGraphic = nullptr;
    }
    
    // Stop collision detection
    if (attackCollisionTimer && attackCollisionTimer->isActive()) {
        attackCollisionTimer->stop();
    }
    
    // Reset attack state
    isAttacking = false;
}

void Player::checkAttackCollision()
{
    // Skip if no attack graphic or no scene
    if (!attackGraphic || !scene()) return;
    
    // Get all items colliding with the attack graphic
    QList<QGraphicsItem*> collidingItems = scene()->collidingItems(attackGraphic);
    
    for (QGraphicsItem* item : collidingItems) {
        // Skip collision with player itself
        if (item == this) continue;

        Enemy* enemy = dynamic_cast<Enemy*>(item);
        if (enemy) {
            enemy->takeDamage(1);  // Deal 1 damage
            xVelocity = -attackDirectionX * attackLaunchMagnitude;
            yVelocity = -attackDirectionY * attackLaunchMagnitude;
            continue;
        }
        
        // Check for platforms and obstacles
        if (dynamic_cast<Platform*>(item) || dynamic_cast<Obstacle*>(item)) {
            // Launch player in the opposite direction
            xVelocity = -attackDirectionX * attackLaunchMagnitude;
            yVelocity = -attackDirectionY * attackLaunchMagnitude;
            
            // End the attack early
            removeAttack();
            break;
        }
    }
    
    // Update the attack graphic position to follow the player
    if (attackGraphic) {
        updateAttackPosition();
    }
}

void Player::updateAttackPosition()
{
    if (!attackGraphic) return;

    // Position the attack in front of the player based on the attack direction
    float offsetX = rect().width() / 2 + attackDirectionX * attackDistance;
    float offsetY = rect().height() / 2 + attackDirectionY * attackDistance;

    attackGraphic->setPos(pos().x() + offsetX, pos().y() + offsetY);
}

QGraphicsPathItem* Player::createAttackGraphic(float dirX, float dirY)
{
    // Create a crescent-shaped path
    QPainterPath path;
    // The size of the crescent
    float radius = 30;
    float thickness = 10;
    // Calculate angle of direction vector
    float angle = std::atan2(dirY, dirX) * 180 / M_PI;

    // Adjust the angle offset to make the crescent points face the player in all directions
    float startAngle;

    // Determine if this is a diagonal direction
    bool isDiagonal = (std::abs(dirX) > 0.1 && std::abs(dirY) > 0.1);

    if (std::abs(dirY) > std::abs(dirX)) {
        // Primarily vertical direction (up or down)
        if (dirY < 0) {
            // Up direction - points should face down (towards player)
            startAngle = angle + 135;
        } else {
            // Down direction - points should face up (towards player)
            startAngle = angle - 135;
        }
    } else if (isDiagonal) {
        // Handle diagonal directions
        if (dirX > 0 && dirY < 0) {
            // Up-right diagonal - FIXED: point to bottom-left
            startAngle = angle + 45;
        } else if (dirX < 0 && dirY < 0) {
            // Up-left diagonal - already working
            startAngle = angle + 225;
        } else if (dirX > 0 && dirY > 0) {
            // Down-right diagonal - FIXED: point to top-left
            startAngle = angle + 225;
        } else {
            // Down-left diagonal - already working
            startAngle = angle + 45;
        }
    } else {
        // Primarily horizontal direction
        startAngle = angle - 45;
    }

    // Create a crescent shape
    path.arcTo(-radius, -radius, radius * 2, radius * 2, startAngle, 90);

    // Create an inner arc to make it hollow
    QPainterPath innerPath;
    innerPath.arcTo(-(radius - thickness), -(radius - thickness),
                    (radius - thickness) * 2, (radius - thickness) * 2,
                    startAngle, 90);

    // Subtract inner arc from outer arc to create crescent
    path = path.subtracted(innerPath);

    // Create the graphic item
    QGraphicsPathItem* attackItem = new QGraphicsPathItem(path);

    // Set position relative to player
    float offsetX = rect().width() / 2 + dirX * attackDistance;
    float offsetY = rect().height() / 2 + dirY * attackDistance;
    attackItem->setPos(pos().x() + offsetX, pos().y() + offsetY);

    // Make it a bright color to contrast with player
    attackItem->setBrush(QBrush(Qt::green));
    attackItem->setPen(QPen(Qt::darkGreen, 2));

    return attackItem;
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
    
    // Reset attack state
    isAttacking = false;
    removeAttack();
    
    // Reset platform state
    isOnGround = false;
    isOnPlatform = false;
    currentPlatform = nullptr;
    
    // Reset color
    setBrush(QBrush(Qt::red));
    
    // Reset health
    health.reset();
    
    // Update UI with reset health
    emit healthChanged(health.get());
}
