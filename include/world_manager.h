#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include "room.h"
#include "player.h"
#include "pause_menu.h"

class WorldManager
{
public:
    WorldManager();

    void Initialize();

    void Update(sf::Time elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

    void LoadSave(sf::RenderWindow& window);
    void Resize(sf::Vector2u ratio, sf::RenderWindow& window);

    void RegisterDeathCallback(std::function<void(void)> f);
    void Death();
    void Resume();
    void QuitToMenu();

private:
    bool checkCollisions(sf::IntRect new_position);
    void changeRoom(sf::Vector2i position);

    void collapseTorch(void*);
    void fallingBoulder(void*);

    void updateFallingBoulderCutscene(sf::Time elapsed, sf::RenderWindow& window);

    std::function<void(void)> deathCallback;

    Room current_room;
    Room new_room;
    std::string cutscene{""};
    sf::Vector2i room_transition{0, 0};
    bool portal_transition{false};
    bool portal_in{false};
    float transition_time{0};
    sf::RectangleShape fade{sf::Vector2f(1200, 800)};
    Player player;
    sf::Time elapsed_seconds;
    PauseMenu pause_menu;
    bool paused{false};
    std::map<std::string, std::list<void*>> trigger_flags;
    std::map<std::string, std::function<void(void*, sf::Time)>> trigger_functions;
};

#endif // WORLD_MANAGER_H
