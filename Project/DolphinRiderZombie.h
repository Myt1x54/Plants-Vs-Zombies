#pragma once
#include "Zombie.h"
#include <SFML/Graphics.hpp>

class DolphinRiderZombie : public Zombie
{
public:
	DolphinRiderZombie(int newHealth, int newAttackDamage, sf::RenderWindow& window);

	void draw();

	void setPosition(float x, float y);

	sf::Vector2f getPosition() const;
};
