#include "Plant.h"

Plant::Plant() : PlantFactory(0, 0, 0, window)
{

}


Plant::Plant(int newCost, int newHealth, int newAttackDamage, RenderWindow& window) : PlantFactory(newCost, newHealth, newAttackDamage, window)
{
	
}

sf::Vector2f Plant::getPosition() const
{
	// do nothing
	return sf::Vector2f(1000000, 1000000);
}

void Plant::updateSprite()
{
	// do nothing
}

void Plant::draw()
{
	// nothing to draw here
}

void Plant::setPosition(float x, float y)
{
	// nothing to set here
}

void Plant::shootPea(ZombieFactory** zombie)
{
	// do nothing
}

void Plant::update()
{
	// do nothing
}

void Plant::generateSun()
{
	// do nothing
}

void Plant::setCurrency(int& currency)
{
	// do nothing
}

bool Plant::isClicked(sf::Vector2f mousePosition)
{
	// do nothing
	return 0;
}