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

void Room::Resize(sf::Vector2u ratio)
{

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

    int x = 0;
    int y = 0;

    while (!file.eof())
    {
        std::getline(file, line);

        std::istringstream iss(line);

        while (iss)
        {
            std::string identifier;
            iss >> identifier;

            if (identifier == "")
            {
                x = 0;
                break;
            }

            if (identifier != ".")
            {
                Entity new_entity(identifier);
                new_entity.GetSprite().setPosition(x, y);
                entities.push_back(new_entity);
            }

            x += 100;
        }
        y += 100;
    }

    return true;
}
