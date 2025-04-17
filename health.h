#ifndef HEALTH_H
#define HEALTH_H

class Health {
public:
    Health(int max = 3) : maxHealth(max), currentHealth(max) {}

    void takeDamage(int amount = 1) {
        currentHealth -= amount;
        if (currentHealth < 0) currentHealth = 0;
    }

    void reset() {
        currentHealth = maxHealth;
    }

    int get() const {
        return currentHealth;
    }

private:
    int maxHealth;
    int currentHealth;
};

#endif // HEALTH_H
