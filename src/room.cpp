#include "room.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "data_file.h"

Room::Room()
{
}

Room::Room(RoomID id) : id{id}
{
}

void Room::Update(sf::Time elapsed, sf::RenderWindow& window, Player& player)
{
    for (auto& entity : entities)
    {
        entity.Update(elapsed, window);
    }

    for (auto& enemy : enemies)
    {
        enemy.Update(elapsed, window, player);
    }
}

void Room::Draw(sf::RenderWindow& window)
{
    background.Draw(window);

    for (auto& entity : entities)
    {
        entity.Draw(window);
    }

    for (auto& enemy : enemies)
    {
        enemy.Draw(window);
    }
}

std::list<Entity>& Room::GetEntities()
{
    return entities;
}

std::list<Enemy>& Room::GetEnemies()
{
    return enemies;
}

RoomID Room::GetID()
{
    return id;
}

bool Room::Load()
{
    std::string filepath = "data/rooms/" + id.area + "/" + std::to_string(id.x) + "-" + std::to_string(id.y) + ".txt";

    DataFile data_file;
    if (!data_file.Open(filepath))
    {
        std::cerr << "Exodus: Room could not be loaded: " << filepath << std::endl;
        return false;
    }

    while (data_file.MoreKeys())
    {
        auto data = data_file.GetKey();
        if (data.key == "Entity")
        {
            auto ss = data.ss;

            std::string identifier;
            int x, y;

            *ss >> identifier;
            *ss >> x;
            *ss >> y;

            Entity entity(identifier);
            entity.GetSprite().setPosition(x, y);
            entities.push_back(entity);
        }
        else if (data.key == "Enemy")
        {
            auto ss = data.ss;

            std::string identifier;
            int x, y;

            *ss >> identifier;
            *ss >> x;
            *ss >> y;

            Enemy enemy(identifier);
            enemy.GetSprite().setPosition(x, y);
            enemies.push_back(enemy);
        }
        else if (data.key == "Background")
        {
            auto ss = data.ss;

            std::string sprite_path;

            *ss >> sprite_path;

            Spritesheet::Config config;
            config.frames.push_back(sf::IntRect(0, 0, 1200, 800));
            background = Spritesheet(sprite_path, config);
            background.SetFrame(0);
        }
    }

    return true;
}
