#ifndef HEALTH_H
#define HEALTH_H

class Health {
public:
    Health(int max = 3);

    void takeDamage(int amount = 1);
    void reset();
    int get() const;

private:
    int maxHealth;
    int currentHealth;
};

#endif // HEALTH_H
