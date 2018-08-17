#include "game_manager.h"

GameManager::GameManager(sf::RenderWindow& window) : current_scene{Scene::MainMenu}, main_menu{}, window{window}
{
    main_menu.RegisterPlayRequest(std::bind(&GameManager::StartGame, this));
    main_menu.RegisterQuitRequest(std::bind(&GameManager::Quit, this));
    main_menu.Resize(sf::Vector2u(Settings::video_resolution.x, Settings::video_resolution.y), window);
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
            world_manager.Update(elapsed, window);
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
            world_manager.Draw(window);
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
    switch (current_scene)
    {
        case Scene::MainMenu:
        {
            main_menu.Resize(ratio, window);
        }
        break;
        case Scene::Game:
        {
            world_manager.Resize(ratio, window);
        }
        break;
        case Scene::Quit:
        {
        }
        break;
    }
}

void GameManager::StartGame()
{
    current_scene = Scene::Game;
    world_manager.LoadSave(window);
}

void GameManager::Quit()
{
    current_scene = Scene::Quit;
}
