#include "world_manager.h"
#include "settings.h"
#include "utilities.h"

#include <iostream>

WorldManager::WorldManager() : current_room{RoomID{"Overworld", 5, 5}}
{
    player.GetSprite().setPosition(sf::Vector2f(500, 500));
}

void WorldManager::Initialize()
{
    player.RegisterCollisionCheck(std::bind(&WorldManager::checkCollisions, this, std::placeholders::_1));
    player.RegisterChangeRoom(std::bind(&WorldManager::changeRoom, this, std::placeholders::_1));
    player.RegisterDeathCallback(std::bind(&WorldManager::Death, this));
}

void WorldManager::Update(sf::Time elapsed, sf::RenderWindow& window)
{
    checkCollisions(sf::IntRect(0, 0, 50, 50));
    elapsed_seconds = elapsed;
    if (room_transition == sf::Vector2i{0, 0})
    {
        current_room.Update(elapsed, window, player);
        player.Update(elapsed, window);
    }
}

void WorldManager::Draw(sf::RenderWindow& window)
{
    if (room_transition == sf::Vector2i{0, 0})
    {
        current_room.Draw(window);
        player.Draw(window);
    }
    else
    {
        float transition_speed = (room_transition.x == 0 ? 400 : 600) * elapsed_seconds.asSeconds();

        sf::View current_room_view = window.getView();
        sf::View new_room_view = window.getView();
        new_room_view.move(1200 * -room_transition.x, 800 * -room_transition.y);

        new_room_view.move(transition_speed * room_transition.x, transition_speed * room_transition.y);
        window.setView(new_room_view);
        new_room.Draw(window);

        current_room_view.move(transition_speed * room_transition.x, transition_speed * room_transition.y);
        window.setView(current_room_view);
        current_room.Draw(window);

        player.Draw(window);

        if (((room_transition.x > 0) ? window.getView().getCenter().x >= 1800 : window.getView().getCenter().x <= -600) ||
            ((room_transition.y > 0) ? window.getView().getCenter().y >= 1200 : window.getView().getCenter().y <= -400))
        {
            current_room = new_room;
            player.GetSprite().move(1200 * -room_transition.x, 800 * -room_transition.y);
            sf::View view = window.getView();
            view.move(1200 * -room_transition.x, 800 * -room_transition.y);
            window.setView(view);
            current_room.Draw(window);
            room_transition.x = 0;
            room_transition.y = 0;
        }
    }
}

void WorldManager::RegisterDeathCallback(std::function<void(void)> f)
{
    deathCallback = f;
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
    float window_ratio = static_cast<float>(ratio.x) / (static_cast<float>(ratio.y) - 72);

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

    sf::FloatRect view_rect{0, 0, 1200, 800};
    view_rect.left = window.getView().getCenter().x - window.getView().getSize().x / 2;
    view_rect.top = window.getView().getCenter().y - window.getView().getSize().y / 2;

    sf::View view(view_rect);
    view.setViewport(sf::FloatRect(viewport_x, viewport_y + (72.0 / 800.0), viewport_width, viewport_height - (72.0 / 800.0)));
    window.setView(view);
    player.SetHudViewport(sf::FloatRect(viewport_x, viewport_y, viewport_width, viewport_height));
}

bool WorldManager::checkCollisions(sf::IntRect new_position)
{
    for (auto& entity : current_room.GetEntities())
    {
        if (entity.HasCollisions())
        {
            if (Utilities::CheckCollision(entity.GetSprite().getGlobalBounds(), sf::FloatRect(new_position)))
            {
                return true;
            }
        }
    }

    for (auto& enemy : current_room.GetEnemies())
    {
        if (enemy.HasCollisions())
        {
            if (Utilities::CheckCollision(enemy.GetSprite().getGlobalBounds(), sf::FloatRect(new_position)))
            {
                return true;
            }
        }
    }

    return false;
}

void WorldManager::changeRoom(sf::Vector2i room_offset)
{
    RoomID id = current_room.GetID();
    id.x += room_offset.x;
    id.y += room_offset.y;
    new_room = Room(id);
    new_room.Load();
    room_transition = room_offset;
}

void WorldManager::Death()
{
    deathCallback();
}
