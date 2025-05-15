#include "health.h"

Health::Health(int max)
    : maxHealth(max), currentHealth(max) {}

void Health::takeDamage(int amount) {
    currentHealth -= amount;
    if (currentHealth < 0) currentHealth = 0;
}

void Health::reset() {
    currentHealth = maxHealth;
}

int Health::get() const {
    return currentHealth;
}
