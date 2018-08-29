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

class Room
{
public:
    Room();
    Room(RoomID id);

    void Update(sf::Time elapsed, sf::RenderWindow& window, Player& player);
    void Draw(sf::RenderWindow& window);

    bool Load();
    std::list<Entity>& GetEntities();
    std::list<Enemy>& GetEnemies();
    std::list<Portal>& GetPortals();
    std::list<Spritesheet>& GetTerrain();
    RoomID GetID();

private:
    RoomID id;
    Spritesheet background;
    std::list<Entity> entities;
    std::list<Enemy> enemies;
    std::list<Portal> portals;
    std::list<Spritesheet> terrain_features;

};

#endif // ROOM_H
