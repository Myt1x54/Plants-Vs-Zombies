#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"

using namespace std;
using namespace sf;

void createBack(RenderWindow& window)
{
    // Drawing the background
    Image map_image;
    map_image.loadFromFile("../Images/background4.png");
    Texture map;
    map.loadFromImage(map_image);
    Sprite s_map;
    s_map.setTexture(map);
    s_map.setPosition(0, 0);
    window.draw(s_map);
}

Game::Game(sf::RenderWindow& window) : window(window)
{
    mainMenu = new MainMenu(window);
    startScreen = new StartScreen(window);
    stagesScreen = new StagesScreen(window);
    instructionScreen = new InstructionScreen(window);
    currentScreen = 0;
}

void Game::run()
{
    while (window.isOpen()) {
        switch (currentScreen) {
        case 0: // Start Screen
            runStartScreen();
            break;
        case 1: // Main Menu
            runMainMenu();
            break;
        case 2: // Stages Screen
            runStagesScreen();
            break;
        case 3:
            runInstructionScreen();
            break;
        default:
            break;
        }
    }
}

void Game::runStartScreen()
{
    startScreen->display(window, *startScreen);

    while (!startScreen->isStartKeyPressed()) {
        handleEvents();
    }

    currentScreen = 1; // Switch to Main Menu after Start Screen
}

void Game::runMainMenu()
{
    mainMenu->display(window, *mainMenu);

    int selectedOption = mainMenu->handleInput();

    switch (selectedOption) {
    case 0: // Start game
        currentScreen = 2; // Switch to Stages Screen
        break;
    case 1: // Show instructions
        currentScreen = 3; // Switch to Instruction Screen
        break;
    case 2: // Show scoreboard
        // Display scoreboard
        break;
    case 3: // Show scoreboard
        window.close();
        break;
    default:
        break;
    }
}

void Game::runInstructionScreen()
{
    currentScreen = instructionScreen->display(window, *instructionScreen);


    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close(); // Close the window if the close button is clicked
        }
        
    }
}

void Game::runStagesScreen()
{
    stagesScreen->display(window, *stagesScreen);
    Levels* levels[6];
    levels[0] = new BeginnersGarden(window);
    levels[1] = new ZombieOutskirts(window);
    levels[2] = new SunflowerFields(window);
    levels[3] = new FoggyForest(window);
    levels[4] = new NighttimeSiege(window);
    levels[5] = new RooftopRampage(window);





    int selectedOption = stagesScreen->handleInput();
    int gameover = 0;
    switch (selectedOption) {
    case 0:
        gameover = levels[0]->display();               // Beginner's Garden
        if (gameover == 1)
        {
            currentScreen = 1; // Switch to Main Menu
            delete stagesScreen; // Close the stage screen
            stagesScreen = new StagesScreen(window); // Recreate the stage screen for next use
            return; // Exit the function and return to the main loop
        }
        break;
    case 1:
        gameover = levels[1]->display();               // Zombies Outskirts
        if (gameover == 1)
        {
            currentScreen = 1; // Switch to Main Menu
            delete stagesScreen; // Close the stage screen
            stagesScreen = new StagesScreen(window); // Recreate the stage screen for next use
            return; // Exit the function and return to the main loop
        }
        break;
    case 2:
        levels[2]->display();               // Sunflower Field
        break;
    case 3:
        levels[3]->display();               // Foggy Forest
        break;
    case 4:
        levels[4]->display();               // Nighttime Siege
        break;
    case 5:
        levels[5]->display();               // Rooftop Rampage
        break;
    case 6:
        currentScreen = 1; // Switch to Main Menu
        delete stagesScreen; // Close the stage screen
        stagesScreen = new StagesScreen(window); // Recreate the stage screen for next use
        return; // Exit the function and return to the main loop
    default:
        break;
    }
}

void Game::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

Game::~Game()
{
    delete startScreen;
    delete mainMenu;
    delete stagesScreen;
    delete instructionScreen;
}