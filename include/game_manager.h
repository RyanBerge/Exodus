#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include "settings.h"
#include "main_menu.h"
#include "world_manager.h"

enum class Scene
{
    MainMenu, Game, Quit
};

class GameManager
{
public:
    GameManager(sf::RenderWindow& render_window);

    void InitializeWorldManager();

    void Update(sf::Time& elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

    void Resize(sf::Vector2u ratio);

    void StartGame();
    void Quit();
    void Death();
    void Fullscreen(bool full);

private:
    Scene current_scene;

    MainMenu main_menu;
    WorldManager world_manager;
    sf::RenderWindow& window;

    bool reset{false};
};

#endif // GAME_MANAGER_H
