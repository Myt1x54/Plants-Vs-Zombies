#pragma once
#include <SFML/Graphics.hpp>
#include "Plant.h"
using namespace std;
using namespace sf;
class CherryBomb : public Plant
{
private:
	sf::Clock GenerateTimer;
	int hitcount;
	bool Destroyed;
public:
	CherryBomb(int newCost, int newHealth, int newAttackDamage, sf::RenderWindow& window);

	void draw();

	void setPosition(float x, float y);

	sf::Vector2f getPosition() const;

	void takeDamage();

	bool isDestroyed();

	void Blast();
};

