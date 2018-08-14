#include "room.h"

#include <fstream>
#include <sstream>
#include <iostream>

Room::Room(RoomID id) : id{id}
{
}

void Room::Update(sf::Time elapsed, sf::RenderWindow& window)
{
    for (auto entity : entities)
    {
        entity.Update(elapsed, window);
    }
}

void Room::Draw(sf::RenderWindow& window)
{
    background.Draw(window);
    for (auto entity : entities)
    {
        entity.Draw(window);
    }
}

std::list<Entity>& Room::GetEntities()
{
    return entities;
}

bool Room::Load()
{
    if (id.area == "Overworld")
    {
        Spritesheet::Config config;
        config.frames.push_back(sf::IntRect(0, 0, 1200, 800));
        background = Spritesheet("assets/Grass.png", config);
        background.SetFrame(0);
    }

    std::string filepath = "data/rooms/" + id.area + "/" + std::to_string(id.x) + "-" + std::to_string(id.y) + ".txt";
    std::ifstream file(filepath);

    if (!file.is_open())
    {
        std::cerr << "Exodus: Room could not be loaded: " << filepath << std::endl;
        return false;
    }

    std::string line;

    while (!file.eof())
    {
        std::getline(file, line);
        if (line == "")
        {
            continue;
        }

        auto sit = line.begin();
        std::string key;
        while (sit != line.end() && *sit != ':')
        {
            key += *sit;
            ++sit;
        }

        while (sit != line.end() && (*sit == ' ' || *sit == ':'))
        {
            ++sit;
        }

        if (key == "Entity")
        {
            std::stringstream ss(std::string(sit, line.end()));
            std::string identifier;
            int x;
            int y;

            ss >> identifier;
            ss >> x;
            ss >> y;

            Entity entity(identifier);
            entity.GetSprite().setPosition(x, y);
            entities.push_back(entity);
        }
    }

    return true;
}
