#ifndef ROOM_H
#define ROOM_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <list>
#include <set>
#include <mutex>
#include "entity.h"

struct RoomID
{
    std::string area;
    int x;
    int y;

    bool operator==(const RoomID& other) const;
    bool operator<(const RoomID& other) const;
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
    std::list<Portal> portals{};
    std::list<WorldTrigger> triggers{};
    std::shared_ptr<sf::RenderTexture> light_layer{new sf::RenderTexture()};
    int light_level{0};

    static std::map<RoomID, std::set<std::string>> dungeon_states;
    static std::mutex state_mutex;
    static void InitDungeonStates();

};

#endif // ROOM_H
