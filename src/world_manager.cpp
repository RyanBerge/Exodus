#include "world_manager.h"
#include "settings.h"

#include <iostream>

WorldManager::WorldManager() : current_room{RoomID{"Overworld", 5, 5}}
{
    player.GetSprite().setPosition(sf::Vector2f(500, 500));
    player.RegisterCollisionCheck(std::bind(&WorldManager::checkCollisions, this, std::placeholders::_1));
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

void WorldManager::LoadSave(sf::RenderWindow& window)
{
    current_room = RoomID{"Overworld", 5, 5};
    current_room.Load();
    Resize(sf::Vector2u(Settings::video_resolution.x, Settings::video_resolution.y), window);
}

void WorldManager::Resize(sf::Vector2u ratio, sf::RenderWindow& window)
{
    float aspect_ratio = 1200.f / 800.f;
    float window_ratio = static_cast<float>(ratio.x) / static_cast<float>(ratio.y);

    float viewport_width = 1;
    float viewport_height = 1;
    float viewport_x = 0;
    float viewport_y = 0;

    if (window_ratio > aspect_ratio)
    {
        viewport_width = aspect_ratio / window_ratio;
        viewport_x = (1 - viewport_width) / 2;
    }
    else if (window_ratio < aspect_ratio)
    {
        viewport_height = window_ratio / aspect_ratio;
        viewport_y = (1 - viewport_height) / 2;
    }

    sf::View view(sf::FloatRect(0, 0, 1200, 800));
    view.setViewport(sf::FloatRect(viewport_x, viewport_y, viewport_width, viewport_height));
    window.setView(view);
}

bool WorldManager::checkCollisions(sf::IntRect new_position)
{
    for (auto& entity : current_room.GetEntities())
    {
        auto bounds = entity.GetSprite().getGlobalBounds();
        if (new_position.left < bounds.left + bounds.width && new_position.left + new_position.width > bounds.left &&
            new_position.top < bounds.top + bounds.height && new_position.top + new_position.height > bounds.top)
        {
            return true;
        }
    }
    return false;
}
