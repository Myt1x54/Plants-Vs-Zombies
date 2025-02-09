#include "ZombieOutskirts.h"



ZombieOutskirts::ZombieOutskirts(RenderWindow& window) : Levels(window)
{
    // Load background image for Zombie Outskirts
    backgroundimage.loadFromFile("../Images/ZombieOutskirts.png");
    backgroundTexture.loadFromImage(backgroundimage);
    backgroundSprite.setTexture(backgroundTexture);
    currency = 500;
    lives = 3;
    font.loadFromFile("../Images/HouseofTerrorRegular.otf"); // Load font file
    currencyText.setFont(font);
    currencyText.setCharacterSize(50);
    sf::Color brownColor(139, 69, 19); // RGB values for brown color
    currencyText.setFillColor(brownColor);
    currencyText.setOutlineColor(sf::Color::Black);
    currencyText.setOutlineThickness(3);
    currencyText.setPosition(320, 35); // position
    livefont.loadFromFile("../Images/HouseofTerrorRegular.otf"); // Load font file
    LivesText.setFont(font);
    LivesText.setCharacterSize(100);
    sf::Color brownlivesColor(139, 69, 19); // RGB values for brown color
    LivesText.setFillColor(brownlivesColor);
    LivesText.setOutlineColor(sf::Color::Black);
    LivesText.setOutlineThickness(3);
    LivesText.setPosition(800, 420); // position
    plant = new PlantFactory * [45];
    zombie = new ZombieFactory * [15];
    gameTime = new GameTime;
}

void ZombieOutskirts::draw()
{
    window.draw(backgroundSprite);
    setCurrency(0);
    window.draw(currencyText);
}

bool ZombieOutskirts::loseLife()
{
    // Get the current elapsed time
    float elapsedTime = gameTime->getElapsedTime();

    // Display the "Remaining Lives" or "Game Over" text based on the remaining lives
    if (lives > 0 && currency < 10000)
    {
        LivesText.setString("Remaining Lives: " + std::to_string(lives));
    }
    else if (lives <= 0 || currency >= 10000)
    {
        LivesText.setString("Game Over");
        LivesText.setPosition(500, 420); // position of the new text

        sf::Text continueText;
        continueText.setFont(font);
        continueText.setCharacterSize(100);
        sf::Color browncontinueTextColor(139, 69, 19); // RGB values for brown color
        continueText.setFillColor(browncontinueTextColor);
        continueText.setOutlineColor(sf::Color::Black);
        continueText.setOutlineThickness(3);
        continueText.setString("Press Space to Continue to Main Menu");
        continueText.setPosition(380, 600); 

        window.draw(LivesText);
        window.draw(continueText);
        window.display();

        // Wait for spacebar key press to continue
        bool spacePressed = false;
        while (!spacePressed)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                    return true; // Indicate game over if window closed
                }
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
                {
                    spacePressed = true;
                }
            }
        }

        return true; // Indicate game over after space pressed
    }

    // Display text
    window.draw(LivesText);
    window.display();

    // Delay for a certain duration before continuing
    float delayDuration = 3.0f; // 3 seconds
    while (elapsedTime < delayDuration)
    {
        // Get the updated elapsed time
        elapsedTime = gameTime->getElapsedTime();

        // Update the window events to prevent freezing
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return false; // Indicate that the game is not over yet
}

int ZombieOutskirts::display()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    sf::Vector2i mousePosition;

    int selectedPlantType = -1;

    bool shovel = false;

    for (int i = 0; i < 45; ++i)
    {
        plant[i] = nullptr;
    }

    for (int i = 0; i < 15; ++i)
    {
        zombie[i] = nullptr;
    }

    bool paused = false;
    bool spacePressedDuringPause = false;

    float count = 0;

    float spawnTime = 15.0f; // Time interval between zombie spawns 15 seconds
    float timeSinceSpawn = 0.0f;
    int zombieCount = 0; // number of zombies spawned

    bool zombiemoving[15];
    for (int i = 0; i < 15; i++)
    {
        zombiemoving[i] = 1;
    }

    int i = 0;
    while (window.isOpen())
    {
        // Get elapsed time since last frame
        float elapsedTime = gameTime->getElapsedTime();
        gameTime->restartClock(); // Restart the clock for the next frame

        // Update time since last spawn
        timeSinceSpawn += elapsedTime;

        count = 0;
        /// Get the current mouse position relative to the window
        mousePosition = sf::Mouse::getPosition(window);

        draw();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // Check if the player clicked on the plant icon
                    if (mousePosition.x >= 30 && mousePosition.x <= 130 && mousePosition.y >= 217 && mousePosition.y <= 329 && !shovel && currency >= 100)
                    {
                        // Set the selected plant type based on the icon clicked
                        selectedPlantType = 1;
                    }
                    else if (mousePosition.x >= 30 && mousePosition.x <= 130 && mousePosition.y >= 355 && mousePosition.y <= 470 && !shovel && currency >= 100)
                    {
                        // Set the selected plant type based on the icon clicked
                        selectedPlantType = 0;
                    }
                    else if (mousePosition.x >= 30 && mousePosition.x <= 130 && mousePosition.y >= 495 && mousePosition.y <= 605 && !shovel && currency >= 150)
                    {
                        // Set the selected plant type based on the icon clicked
                        selectedPlantType = 2;
                    }
                    else if (mousePosition.x >= 30 && mousePosition.x <= 130 && mousePosition.y >= 630 && mousePosition.y <= 738 && !shovel && currency >= 50)
                    {
                        // Set the selected plant type based on the icon clicked
                        selectedPlantType = 3;
                    }
                    // Check if the player clicked on the grid
                    else
                    {
                        const float horizontalDistance = (1870 - 480) / 9.0f;
                        const float verticalDistance = (1034 - 125) / 5.0f;

                        // Calculate the row and column for the clicked position
                        int row = (event.mouseButton.y - 125) / verticalDistance;
                        int column = (event.mouseButton.x - 480) / horizontalDistance;

                        // Calculate the position of the new plant based on the row and column
                        float plantX = 420 + column * horizontalDistance + horizontalDistance / 2;
                        float plantY = 75 + row * verticalDistance + verticalDistance / 2;


                        if (mousePosition.x >= 30 && mousePosition.x <= 130 && mousePosition.y >= 25 && mousePosition.y <= 129)
                        {
                            // Set the selected plant type as shovel
                            if (shovel)
                            {
                                shovel = false;
                                selectedPlantType = -1;
                            }
                            else
                            {
                                shovel = true;
                                selectedPlantType = -1;
                            }
                        }
                        if (shovel)
                        {   // If shovel is selected
                            // Find the clicked plant and remove it
                            for (int j = 0; j < i; ++j)
                            {
                                if (plant[j] != nullptr && plant[j]->getPosition() == sf::Vector2f(plantX, plantY))
                                {
                                    delete plant[j]; // Delete the plant object

                                    // Shift the subsequent elements to fill the gap
                                    shovel = false;
                                    for (int k = j; k < i - 1; ++k)
                                    {
                                        plant[k] = plant[k + 1];
                                    }
                                    plant[i - 1] = nullptr;             // Set the last element to nullptr
                                    --i;                                // Decrease the plant count
                                    break;                              // Exit the loop after removing the plant
                                }
                            }
                        }

                        else if (selectedPlantType != -1 && i < 45 && !shovel)
                        {

                            bool insideGrid = false;

                            const float horizontalDistance = (1870 - 480) / 9.0f;
                            const float verticalDistance = (1034 - 125) / 5.0f;

                            // Check if the mouse click occurred inside the grid
                            if (event.mouseButton.x >= 480 && event.mouseButton.x <= 1870 &&
                                event.mouseButton.y >= 125 && event.mouseButton.y <= 1034) {
                                insideGrid = true;

                                // Calculate the row and column for the clicked position
                                int row = std::max(0, std::min(4, static_cast<int>((event.mouseButton.y - 125) / verticalDistance)));
                                int column = std::max(0, std::min(8, static_cast<int>((event.mouseButton.x - 480) / horizontalDistance)));

                                // Calculate the position of the new plant based on the row and column
                                float plantX = 420 + column * horizontalDistance + horizontalDistance / 2;
                                float plantY = 75 + row * verticalDistance + verticalDistance / 2;
                            }

                            bool positionOccupied = false;

                            for (int j = 0; j < i; ++j)
                            {
                                if (plant[j] != nullptr && plant[j]->getPosition() == sf::Vector2f(plantX, plantY))
                                {
                                    positionOccupied = true;
                                    break;
                                }
                            }

                            int newIndex = -1;
                            for (int c = 0; c < 45; ++c)
                            {
                                if (plant[c] == nullptr)
                                {
                                    newIndex = c;
                                    break;
                                }
                            }

                            if (!positionOccupied && insideGrid && newIndex != -1)
                            {
                                switch (selectedPlantType)
                                {
                                case 0: // Peashooter
                                    plant[i] = new Peashooter(100, 125, 25, window);
                                    currency -= 100;
                                    selectedPlantType = -1;
                                    break;

                                case 1: // Sunflower
                                    plant[i] = new Sunflower(100, 125, 25, window);
                                    currency -= 100;
                                    selectedPlantType = -1;
                                    break;
                                case 2: // CherryBomb
                                    plant[i] = new CherryBomb(100, 125, 25, window);
                                    currency -= 150;
                                    selectedPlantType = -1;
                                    break;
                                case 3: // WallNut
                                    plant[i] = new Wallnut(100, 125, 25, window, plantX, plantY);
                                    currency -= 50;
                                    selectedPlantType = -1;
                                    break;
                                }
                                // Set the position of the new plant
                                plant[i]->setPosition(plantX, plantY);
                                i++;
                            }
                        }
                    }
                }
            }
            
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    paused = !paused;
                }
                else if (event.key.code == sf::Keyboard::Space && paused)
                {
                    spacePressedDuringPause = true;
                }
            }
        }

        if (paused)
        {
            sf::Text pauseText;
            pauseText.setFont(font);
            pauseText.setCharacterSize(100);
            sf::Color pauseColor(139, 69, 19);
            pauseText.setFillColor(pauseColor);
            pauseText.setOutlineColor(sf::Color::Black);
            pauseText.setOutlineThickness(3);
            pauseText.setString("Game Paused");
            pauseText.setPosition(800, 420);

            sf::Text mainMenuText;
            mainMenuText.setFont(font);
            mainMenuText.setCharacterSize(100);
            sf::Color mainMenuColor(139, 69, 19);
            mainMenuText.setFillColor(mainMenuColor);
            mainMenuText.setOutlineColor(sf::Color::Black);
            mainMenuText.setOutlineThickness(3);
            mainMenuText.setString("Press Space to Go to Main Menu");
            mainMenuText.setPosition(500, 520);

            if (spacePressedDuringPause)
            {
                spacePressedDuringPause = false;
                return 1;
            }

            window.draw(mainMenuText);
            window.draw(pauseText);

            window.display();

            continue;
        }

        if (timeSinceSpawn >= spawnTime && zombieCount < 15)
        {
            int noob = (rand() % 3) + 1;
            // Use the ZombieFactory to create a new zombie instance
            ZombieFactory* newZombie = nullptr;
            switch (noob)
            {
            case 1:
                newZombie = new SimpleZombie(100, 10, window);
                break;
            case 2:
                newZombie = new FootballZombie(100, 10, window);
                break;
            case 3:
                newZombie = new DancingZombie(100, 10, window);
                break;
            default:
                break;
            }

            int randomRow = rand() % 5; // Choose a random row index between 0 and 4
            const float verticalDistance = (1034 - 125) / 5.0f;

            float zombieY = 30 + randomRow * verticalDistance + verticalDistance / 2;

            newZombie->setPosition(1900 + (rand() + 1) % 100, zombieY);

            // Add the zombie to the zombie array
            zombie[zombieCount++] = newZombie;

            // Reset the spawn timer
            timeSinceSpawn = 0.0f;
        }



        if (shovel)
        {
            window.setMouseCursorVisible(false);
            sf::Texture mouseTexture;
            sf::Sprite mouseSprite;
            mouseTexture.loadFromFile("../Images/Shovel2.png");
            mouseSprite.setTexture(mouseTexture);


            mouseSprite.setPosition(mousePosition.x - 90, mousePosition.y - 90);


            window.draw(mouseSprite);
        }
        else if (selectedPlantType == 0)                // Peashooter
        {
            window.setMouseCursorVisible(false);
            sf::Texture mouseTexture;
            sf::Sprite mouseSprite;
            mouseTexture.loadFromFile("../Images/PeaShooterSprite.png");
            mouseSprite.setTexture(mouseTexture);

            mouseSprite.setPosition(mousePosition.x - 40, mousePosition.y - 80);
            window.draw(mouseSprite);
        }

        else if (selectedPlantType == 1)                // Sunflower
        {
            window.setMouseCursorVisible(false);
            sf::Texture mouseTexture;
            sf::Sprite mouseSprite;
            mouseTexture.loadFromFile("../Images/SunflowerSprite.png");
            mouseSprite.setTexture(mouseTexture);

            mouseSprite.setPosition(mousePosition.x - 70, mousePosition.y - 80);
            window.draw(mouseSprite);
        }
        else if (selectedPlantType == 2)                // CherryBomb
        {
            window.setMouseCursorVisible(false);
            sf::Texture mouseTexture;
            sf::Sprite mouseSprite;
            mouseTexture.loadFromFile("../Images/CherryBombSprite.png");
            mouseSprite.setTexture(mouseTexture);

            mouseSprite.setPosition(mousePosition.x - 70, mousePosition.y - 80);
            window.draw(mouseSprite);
        }
        else if (selectedPlantType == 3)                // Wallnut
        {
            window.setMouseCursorVisible(false);
            sf::Texture mouseTexture;
            sf::Sprite mouseSprite;
            mouseTexture.loadFromFile("../Images/WallnutSprite.png");
            mouseSprite.setTexture(mouseTexture);

            mouseSprite.setPosition(mousePosition.x - 70, mousePosition.y - 80);
            window.draw(mouseSprite);
        }
        else
        {
            window.setMouseCursorVisible(true);
        }

        for (int i = 0; i < 15; ++i)
        {
            if (zombie[i] != nullptr)
            {
                if (zombiemoving[i])
                {
                    zombie[i]->Move();
                }
                zombie[i]->updateSprite();
                zombie[i]->draw();
                sf::Vector2f zomposition = zombie[i]->getPosition();
                if (zomposition.x <= 370 && zomposition.y >= 100 && zomposition.x <= 370 && zomposition.y <= 1031 || currency >= 10000)
                {
                    if (currency >= 10000)
                    {
                        bool gameover = loseLife();
                        if (gameover)
                        {
                            return 1;
                        }
                    }
                    else if (zomposition.x <= 370 && zomposition.y >= 100 && zomposition.x <= 370 && zomposition.y <= 1031)
                    {
                        lives--;
                        zombie[i]->DeleteZombie();
                        bool gameover = loseLife();
                        if (gameover)
                        {
                            return 1;
                        }
                    }
                }
            }
        }




        // Generate suns for all sunflowers and draw them
        for (int i = 0; i < 45; i++)
        {
            if (plant[i] != nullptr)
            {
                if (dynamic_cast<Sunflower*>(plant[i]) != nullptr)
                {
                    Sunflower* sunflower = dynamic_cast<Sunflower*>(plant[i]);
                    sunflower->draw(); // Draw the sunflower (and the sun if it's ready)
                    sunflower->generateSun(); // Generate suns for each sunflower

                }
            }
        }

        // check for clicks on the suns
        for (int i = 0; i < 45; i++)
        {
            if (plant[i] != nullptr)
            {
                plant[i]->updateSprite();
                plant[i]->draw();

                // Check if the current plant is a Wallnut
                if (dynamic_cast<Wallnut*>(plant[i]) != nullptr)
                {
                    Wallnut* wallnut = dynamic_cast<Wallnut*>(plant[i]);

                    // Check if the Wallnut hasn't finished moving
                    if (!wallnut->isFinishedMoving())
                    {
                        wallnut->Roll();
                    }
                }

                // Check if the current plant is a Sunflower
                if (dynamic_cast<Sunflower*>(plant[i]) != nullptr)
                {
                    Sunflower* sunflower = dynamic_cast<Sunflower*>(plant[i]);

                    // Check if the sun is clicked and add currency
                    if (sunflower->isClicked(static_cast<Vector2f>(mousePosition)))
                    {
                        sunflower->setCurrency(currency);
                    }
                }
            }
        }
        for (int j = 0; j < i; ++j)
        {
            if (plant[j] != nullptr && dynamic_cast<Peashooter*>(plant[j]) != nullptr)
            {
                Peashooter* peashooter = dynamic_cast<Peashooter*>(plant[j]);

                // Get the position of the Peashooter
                sf::Vector2f peashooterPosition = plant[j]->getPosition();

                // Iterate through zombies to find if there's one in the same row as the Peashooter
                for (int k = 0; k < 15; ++k)
                {
                    if (zombie[k] != nullptr)
                    {
                        sf::Vector2f zombiePosition = zombie[k]->getPosition();

                        // Calculate the Y-position for the zombie
                        int randomRow = rand() % 5; // Choose a random row index between 0 and 4
                        const float verticalDistance = (1034 - 125) / 5.0f;

                        float zombieY = 30 + randomRow * verticalDistance + verticalDistance / 2;

                        // Check if the zombie is in the same row as the Peashooter
                        if (zombiePosition.x <= 1900 && abs(zombiePosition.y - peashooterPosition.y) < verticalDistance / 2)
                        {
                            // If yes, shoot at the zombie
                            peashooter->shootPea(zombie, 15);
                        }
                    }
                }
            }
        }
       

        for (int j = 0; j < i; ++j)
        {
            bool plantDestroyed = false; // Flag to indicate if a plant has been destroyed
            if (plant[j] != nullptr)
            {
                for (int k = 0; k < zombieCount; ++k)
                {
                    if (zombie[k] != nullptr)
                    {
                        if (zombie[k]->checkCollision(plant[j]->getPosition()))
                        {
                            zombiemoving[k] = 0;
                            plant[j]->takeDamage();

                            if (plant[j]->isDestroyed())
                            {
                                zombiemoving[k] = 1;
                                delete plant[j];
                                for (int k = j; k < i - 1; ++k)
                                {
                                    plant[k] = plant[k + 1];
                                }
                                plant[i - 1] = nullptr;
                                --i;
                                plantDestroyed = true;
                                break;
                            }
                        }
                    }
                }
            }
            if (plantDestroyed)
            {
                break;
            }
        }

        window.display();

    }

    return -1; // Indicate no option selected
}

void ZombieOutskirts::setCurrency(int value)
{
    currencyText.setString(std::to_string(currency));
}

ZombieOutskirts::~ZombieOutskirts()
{
    // Deallocate memory for plant
    for (int i = 0; i < 45; ++i) 
    {
        delete plant[i];
    }
    delete[] plant;

    for (int i = 0; i < 15; ++i)
    {
        delete zombie[i];
    }
    delete[] zombie;
}