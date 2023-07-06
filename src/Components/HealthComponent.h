#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

struct HealthComponent
{
    int maxHealth;
    int currentHealth;

    HealthComponent(int maxHealth = 1)
    {
        this->maxHealth = maxHealth;
        this->currentHealth = maxHealth;
    }
};

#endif