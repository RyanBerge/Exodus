#include "enemy.h"

#include "utilities.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

#define SPINY_SPEED 60

Enemy::Enemy(std::string identifier)
{
    load("data/enemies/" + identifier + ".txt");
}

void Enemy::load(std::string filepath)
{
    std::ifstream file(filepath);
    std::string line;

    if (!file.is_open())
    {
        std::cerr << "Exodus: Enemy was not loaded: " << filepath << std::endl;
        return;
    }

    std::string sprite_path("assets/");
    Spritesheet::Config config;

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
        else if (key == "Behavior")
        {
            if (std::string(sit, line.end()) == "None")
            {
                behavior = Behavior::None;
            }
            else if (std::string(sit, line.end()) == "Spiny")
            {
                behavior = Behavior::Spiny;
            }
        }
        else if (key == "Damage")
        {
            std::stringstream ss(std::string(sit, line.end()));
            ss >> damage;
        }
        else if (key == "Knockback")
        {
            std::stringstream ss(std::string(sit, line.end()));
            ss >> knockback;
        }
    }

    sprite = Spritesheet(sprite_path, config);
}

void Enemy::Update(sf::Time elapsed, sf::RenderWindow& window, Player& player)
{
    sprite.Update(elapsed, window);
    switch (behavior)
    {
        case Behavior::None:
        {
        }
        case Behavior::Spiny:
        {
            spinyUpdate(elapsed, window, player);
        }
    }
}

void Enemy::spinyUpdate(sf::Time elapsed, sf::RenderWindow& window, Player& player)
{
    auto direction = follow(player.GetSprite().getPosition(), elapsed);
    if (Utilities::CheckCollision(sprite.GetSprite().getGlobalBounds(), player.GetSprite().getGlobalBounds()))
    {
        player.Damage(damage, knockback, direction);
    }
}

sf::Vector2f Enemy::follow(sf::Vector2f player_position, sf::Time elapsed)
{
    sf::Vector2f position = sprite.GetSprite().getPosition();
    sf::Vector2f direction = player_position - position;
    direction = Utilities::Normalize(direction);

    sprite.GetSprite().move(direction.x * SPINY_SPEED * elapsed.asSeconds(), direction.y * SPINY_SPEED * elapsed.asSeconds());
    return direction;
}

void Enemy::Draw(sf::RenderWindow& window)
{
    sprite.Draw(window);
}

sf::Sprite& Enemy::GetSprite()
{
    return sprite.GetSprite();
}
