#include "game_manager.h"
#include "global.h"

GameManager::GameManager(sf::RenderWindow& window) : current_scene{Scene::MainMenu}, main_menu{}, window{window}
{
    Global::Play = std::bind(&GameManager::StartGame, this);
    Global::Fullscreen = std::bind(&GameManager::Fullscreen, this, std::placeholders::_1);
    Global::Death = std::bind(&GameManager::Death, this);
    Global::Quit = std::bind(&GameManager::Quit, this);
    Global::QuitToMenu = std::bind(&GameManager::Death, this);

    main_menu.Resize(sf::Vector2u(Settings::video_resolution.x, Settings::video_resolution.y), window);

    InitializeWorldManager();
}

void GameManager::InitializeWorldManager()
{
    Resize(window.getSize());
    world_manager.~WorldManager();
    new(&world_manager) WorldManager();
    world_manager.Initialize();
    reset = false;
}

void GameManager::Update(sf::Time& elapsed, sf::RenderWindow& window)
{
    if (reset)
    {
        InitializeWorldManager();
        sf::View view(sf::Vector2f(600, 400), sf::Vector2f(1200, 800));
        window.setView(view);
        Resize(window.getSize());
    }

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

void GameManager::Death()
{
    current_scene = Scene::MainMenu;
    reset = true;
}

void GameManager::Fullscreen(bool full)
{
    if (full)
    {
        window.create(sf::VideoMode::getDesktopMode(), "Exodus", sf::Style::Fullscreen);
        Resize(window.getSize());
    }
    else
    {
        window.create(sf::VideoMode(Settings::video_resolution.x, Settings::video_resolution.y), "Exodus");
        Resize(window.getSize());
    }
}
