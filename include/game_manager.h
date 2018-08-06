#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include "settings.h"
#include "main_menu.h"

enum class Scene
{
    MainMenu, Game, Quit
};

class GameManager
{
public:
    GameManager();

    void Update(sf::Time& elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

    void Resize(sf::Vector2u ratio);

    void StartGame();
    void Quit();

private:
    Scene current_scene;

    MainMenu main_menu;
};

#endif // GAME_MANAGER_H
