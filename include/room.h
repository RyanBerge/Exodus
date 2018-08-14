#ifndef ROOM_H
#define ROOM_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <list>
#include "entity.h"

struct RoomID
{
    std::string area;
    int x;
    int y;
};

class Room
{
public:
    Room();
    Room(RoomID id);

    void Update(sf::Time elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

    bool Load();
    std::list<Entity>& GetEntities();
    RoomID GetID();

private:
    RoomID id;
    Spritesheet background;
    std::list<Entity> entities;

};

#endif // ROOM_H
