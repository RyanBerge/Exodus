#include "world_manager.h"

WorldManager::WorldManager() : current_room{RoomID{"Overworld", 5, 5}}
{
    player.GetSprite().setPosition(sf::Vector2f(500, 500));
}

void WorldManager::Update(sf::Time elapsed, sf::RenderWindow& window)
{
    current_room.Update(elapsed, window);
    player.Update(elapsed, window);
}

void WorldManager::Draw(sf::RenderWindow& window)
{
    current_room.Draw(window);
    player.Draw(window);
}

void WorldManager::LoadSave()
{
    current_room = RoomID{"Overworld", 5, 5};
    current_room.Load();
}

void WorldManager::Resize(sf::Vector2u ratio)
{
    current_room.Resize(ratio);
}

