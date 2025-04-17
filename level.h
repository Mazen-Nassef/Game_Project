#ifndef LEVEL_H
#define LEVEL_H

class Level
{
private:
    int current_level;
    double frequency_multiplier;
    double velocity_multiplier;
    double damage_multiplier;

    void set_frequencyM();
    void set_velocityM();
    void set_damageM();
public:
    Level();
    int get_level() const;
    double get_frequencyM() const;
    double get_velocityM() const;
    double get_damageM() const;
    void update_level();

};

#endif // LEVEL_H
