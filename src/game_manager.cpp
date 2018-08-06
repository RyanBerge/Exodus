#include "game_manager.h"

GameManager::GameManager() : current_scene{Scene::MainMenu}, main_menu{}
{
    main_menu.RegisterPlayRequest(std::bind(&GameManager::StartGame, this));
    main_menu.RegisterQuitRequest(std::bind(&GameManager::Quit, this));
}

void GameManager::Update(sf::Time& elapsed, sf::RenderWindow& window)
{
    switch (current_scene)
    {
        case Scene::MainMenu:
        {
            main_menu.Update(elapsed, window);
        }
        break;
        case Scene::Game:
        {

        }
        break;
        case Scene::Quit:
        {
            window.close();
            // TODO: Save?
            return;
        }
        break;
    }
}

void GameManager::Draw(sf::RenderWindow& window)
{
    switch (current_scene)
    {
        case Scene::MainMenu:
        {
            main_menu.Draw(window);
        }
        break;
        case Scene::Game:
        {

        }
        break;
        case Scene::Quit:
        {
        }
        break;
    }
}

void GameManager::Resize(sf::Vector2u ratio)
{
    main_menu.Resize(ratio);
}

void GameManager::StartGame()
{
    current_scene = Scene::Game;
}

void GameManager::Quit()
{
    current_scene = Scene::Quit;
}
