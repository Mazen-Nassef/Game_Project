#include <QBrush>
#include "player.h"


Player::Player()
    : health(3), speed(1), maxSpeed(5), friction(0.5), gravity(1), dashSpeed(10), dashDuration(150), length(50), width(30), jumpHeight(15),
    yVelocity(0), xVelocity(0), canDash(true), gravityTimer(new QTimer(this))
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
    case Qt::Key_Down:
        break;
    case Qt::Key_Space:
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
    if (y() >= 500)
    // 500 is the arbitrary value i chose for the floor so i can test the player
    // this will need to be updated based on the level logic
    {
        yVelocity = -jumpHeight;
    } // increments player's vertical velocity by the jump height value (negative for up)
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

    if (keysHeld.contains(Qt::Key_Space) && y() >= 500) {
        jump();
        // jumps if you're holding space and are on the floor, need to update the 500 bit when moaz does the level logic
    }
    if (!isDashing) {
        yVelocity += gravity;
        // applies gravity while not dashing
    }


    if (keysHeld.contains(Qt::Key_Left)) {
        moveBackward();
        //moves backward if holding backward
    }
    if (keysHeld.contains(Qt::Key_Right)) {
        moveForward();
        //moves forward if holding forward
    }


    setX(x() + xVelocity + dashX);
    setY(y() + yVelocity + dashY);
    // dash logic


    if (y() >= 500) {
        setY(500);
        yVelocity = 0;
        //prevents player from falling through the floor, needs to be updated with level logic
        if (!canDash && !isDashing) {
            canDash = true;
            setBrush(QBrush(Qt::red));
        }
        // resets dash
        isDashing = false;
        dashX = 0; dashY = 0;
        //stops dashing

        if (dashTimer && dashTimer->isActive()) {
            dashTimer->stop();
        }
        // chatgpt, i can't figure out timers
    }


    if (!isDashing) {
        float currentFriction = friction;

        if (std::abs(xVelocity) > maxSpeed) {
            currentFriction += (std::abs(xVelocity) - maxSpeed) * 0.2;
        } // if player velocity is higher than the max speed, the friction increases to force them gradually back to the max speed

        if (xVelocity > 0) {
            xVelocity -= currentFriction;
            //applies friction to slow down the player
            if (xVelocity < 0) xVelocity = 0;
            // stops at 0
        } else if (xVelocity < 0) {
            xVelocity += currentFriction;
            //same but in the negative direction
            if (xVelocity > 0) xVelocity = 0;
            // stops at zero

            // could probably combine this with a few absolute functions, but i'm tired rn, i'll do that later
        }
    }
}
