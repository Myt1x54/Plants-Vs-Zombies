#include "Sunflower.h"
#include<iostream>
using namespace std;

Sunflower::Sunflower(int newCost, int newHealth, int newAttackDamage, sf::RenderWindow& window) : Plant(newCost, newHealth, newAttackDamage, window)
{
    // Load background image for BeginnersGarden
    plantImage.loadFromFile("../Images/SunflowerSprite.png");
    plantTexture.loadFromImage(plantImage);
    plantSprite.setTexture(plantTexture);

    // Load sun texture
    sunTexture.loadFromFile("../Images/sun.png");
    sunSprite.setTexture(sunTexture);

    sunCollected = false;

    // Initialize sun count and timer
    sunCount = 0;
     // Start the generateTimer
    generateTimer.restart();

    hitCount = 0;

    destroyed = false;

    lastHitTime = 0;
}

void Sunflower::setPosition(float x, float y)
{
    plantSprite.setPosition(x, y);
    // Set position for sun sprite
    sunSprite.setPosition(x + 50, y - 40);
}

sf::Vector2f Sunflower::getPosition() const
{
    return plantSprite.getPosition();
}

void Sunflower::generateSun()
{
    // Generate sun every 10 seconds
    if (generateTimer.getElapsedTime().asSeconds() >= 10)
    {
        sunCount++;
    }
}

void Sunflower::draw()
{
    window.draw(plantSprite);

    // Draw sun sprite if sun is ready to be generated
    if (generateTimer.getElapsedTime().asSeconds() >= 10)
    {
        window.draw(sunSprite);
    }
}

void Sunflower::setCurrency(int& currency)
{
    // Add 50 currency when sun is clicked
    currency += 50;
}

bool Sunflower::isClicked(sf::Vector2f mousePosition)
{
    // Check if mouse position is within the bounds of the sun sprite
    if (sunSprite.getGlobalBounds().contains(mousePosition))
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            // Check if the sun has not been collected in this frame
            if (!sunCollected && sunCount > 0)
            {
                sunCount--;
                sunCollected = true; // sun has been collected in this frame
                generateTimer.restart();
                return true;
            }
        }
    }
    else
    {
        sunCollected = false; // Reset the flag if the mouse is not over the sun sprite
    }
    return false;
}

void Sunflower::takeDamage()
{
    if (generateTimer.getElapsedTime().asSeconds() >= 3.0f)
    {
        // Increment hitCount only if it's less than 10
        if (hitCount < 3)
        {
            hitCount++;
        }
        generateTimer.restart(); // Restart the timer
    }

    if (hitCount >= 3)
    {
        destroyed = true;
    }
}

bool Sunflower::isDestroyed()
{
    return destroyed; // Return the value of the destroyed flag
}

float Sunflower::getLastHitTime() const
{
    return lastHitTime;
}

void Sunflower::setLastHitTime(float time)
{
    lastHitTime = time;
}