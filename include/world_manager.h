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

    void Update(sf::Time elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

    void LoadSave(sf::RenderWindow& window);
    void Resize(sf::Vector2u ratio, sf::RenderWindow& window);

private:
    bool checkCollisions(sf::IntRect new_position);

    Room current_room;
    Player player;
};

#endif // WORLD_MANAGER_H
