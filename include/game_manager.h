#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include "settings.h"
#include "main_menu.h"

enum class Scene
{
    MainMenu, Game
};

class GameManager
{
public:
    GameManager(Settings& settings);

    void Update(sf::Time& elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

private:
    Settings settings;
    Scene current_scene;

    MainMenu main_menu;
};

#endif // GAME_MANAGER_H
