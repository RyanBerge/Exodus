#ifndef ROOM_H
#define ROOM_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <list>
#include "entity.h"
#include "enemy.h"

struct RoomID
{
    std::string area;
    int x;
    int y;
};

struct Portal
{
    RoomID id;
    sf::FloatRect hitbox;
    sf::Vector2f spawn;
};

struct WorldTrigger
{
    std::string callback_key;
    sf::FloatRect hitbox;
};

class Room
{
public:
    Room();
    Room(RoomID id);

    void Update(sf::Time elapsed, sf::RenderWindow& window, Player& player);
    void Draw(sf::RenderWindow& window);
    void DrawLighting(sf::RenderWindow& window);

    bool Load();

    RoomID id{};
    Spritesheet background{};
    std::list<Entity> entities{};
    std::list<Enemy> enemies{};
    std::list<Portal> portals{};
    std::list<WorldTrigger> triggers{};
    std::shared_ptr<sf::RenderTexture> light_layer{new sf::RenderTexture()};
    int light_level{0};

};

#endif // ROOM_H
