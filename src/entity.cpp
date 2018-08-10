#include "entity.h"

#include <cstdlib>
#include <fstream>
#include <sstream>

Entity::Entity(std::string identifier)
{
    load("data/entities/" + identifier + ".txt");

    // if (identifier == "tree")
    // {
    //     Spritesheet::Config config;
    //     config.frames.push_back(sf::IntRect(0, 0, 100, 100));
    //     config.frames.push_back(sf::IntRect(100, 0, 100, 100));
    //     config.frames.push_back(sf::IntRect(200, 0, 100, 100));
    //     sprite = Spritesheet("assets/OverworldTree.png", config);

    //     sprite.SetFrame(rand() % 3);
    // }
}

void Entity::load(std::string filepath)
{
    std::ifstream file(filepath);
    std::string line;

    std::string sprite_path("assets/");
    Spritesheet::Config config;
    bool random_frame;

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

        if (key == "Sprite")
        {
            sprite_path += std::string(sit, line.end());
        }
        else if (key == "Collisions")
        {
            if (std::string(sit, line.end()) == "True")
            {
                collisions = true;
            }
            else if (std::string(sit, line.end()) == "False")
            {
                collisions = false;
            }
        }
        else if (key == "Frames")
        {
            while (sit != line.end())
            {
                while (sit != line.end() && *sit != '[')
                {
                    ++sit;
                }

                if (sit == line.end())
                {
                    break;
                }

                auto s_sit = ++sit;

                while (sit != line.end() && *sit != ']')
                {
                    ++sit;
                }

                std::stringstream ss(std::string(s_sit, sit));
                int left, right, width, height;
                ss >> left;
                ss >> right;
                ss >> width;
                ss >> height;
                config.frames.push_back(sf::IntRect(left, right, width, height));
            }
        }
        else if (key == "RandomFrame")
        {
            if (std::string(sit, line.end()) == "True")
            {
                random_frame = true;
            }
            else if (std::string(sit, line.end()) == "False")
            {
                random_frame = false;
            }
        }
    }

    sprite = Spritesheet(sprite_path, config);
    if (random_frame)
    {
        sprite.SetFrame(rand() % config.frames.size());
    }
}

void Entity::Update(sf::Time elapsed, sf::RenderWindow& window)
{

}

void Entity::Draw(sf::RenderWindow& window)
{
    sprite.Draw(window);
}

sf::Sprite& Entity::GetSprite()
{
    return sprite.GetSprite();
}
