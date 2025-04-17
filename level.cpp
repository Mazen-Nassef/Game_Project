
#include "level.h"


Level::Level() {
    current_level=1;
    set_frequencyM();
    set_velocityM();
    set_damageM();
}

int Level::get_level () const
{
    return current_level;
}

double Level::get_damageM () const
{
    return damage_multiplier;
}

double Level::get_frequencyM () const
{
    return frequency_multiplier;
}

double Level::get_velocityM () const
{
    return velocity_multiplier;
}

void Level::update_level ()
{
    current_level++;
    set_frequencyM();
    set_velocityM();
    set_damageM();
}

void Level::set_frequencyM() {
    frequency_multiplier = 1.0 + 0.2 * (current_level - 1);
}

void Level::set_velocityM() {
    velocity_multiplier = 1.0 + 0.15 * (current_level - 1);
}

void Level::set_damageM() {
    damage_multiplier = 1.0 + 0.1 * (current_level - 1);
}















