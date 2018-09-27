#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <mutex>
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

    void Death();
    void Resume();

private:
    void loadAdjacentRooms();
    void loadRoom(RoomID id);
    void setNewRoom(RoomID id);

    sf::Vector2f movePlayer(sf::FloatRect hitbox, sf::Vector2f displacement);
    sf::Vector2f moveEntity(sf::FloatRect hitbox, sf::Vector2f displacement, Entity& original_entity);
    void changeRoom(sf::Vector2i position);

    void collapseTorch(void*);
    void fallingBoulder(void*);

    void updateFallingBoulderEvent(sf::Time elapsed, sf::RenderWindow& window);

    std::function<void(void)> deathCallback;

    std::shared_ptr<Room> current_room;
    std::shared_ptr<Room> new_room;
    std::list<std::shared_ptr<Room>> adjacent_rooms;
    std::mutex room_mutex;
    std::string game_event{""};
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
