#ifndef UTILITES_H
#define UTILITES_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "room.h"

namespace Utilities
{
    sf::Vector2f Normalize(sf::Vector2f vector);
    bool CheckCollision(sf::FloatRect left, sf::FloatRect right);
    Spritesheet::Animation ReadAnimation(std::stringstream& ss);
    std::vector<std::list<Spritesheet::LightConfig>> ReadLights(std::stringstream& ss);
    bool CheckDungeonState(std::string dungeon_state, RoomID id);
    void AddDungeonState(std::string dungeon_state, RoomID id);
    void RemoveDungeonState(std::string dungeon_state, RoomID id);
    void SetAllDungeonStates(std::string dungeon_state, RoomID id);
}

#endif // UTILITIES_H
