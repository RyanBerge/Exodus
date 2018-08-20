#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include "room.h"
#include "player.h"

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

private:
    bool checkCollisions(sf::IntRect new_position);
    void changeRoom(sf::Vector2i position);

    std::function<void(void)> deathCallback;

    Room current_room;
    Room new_room;
    sf::Vector2i room_transition{0, 0};
    Player player;
    sf::Time elapsed_seconds;
};

#endif // WORLD_MANAGER_H
