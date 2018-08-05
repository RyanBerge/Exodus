#include "game_manager.h"

GameManager::GameManager(Settings& settings) : settings{settings}, current_scene{Scene::MainMenu}, main_menu{}
{
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
    }
}
