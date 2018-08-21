#include "enemy.h"

#include "utilities.h"
#include "data_file.h"

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
    DataFile data_file;
    if (!data_file.Open(filepath))
    {
        std::cerr << "Exodus: Enemy could not be loaded: " << filepath << std::endl;
        return;
    }

    Spritesheet::Config config;
    std::string sprite_path("assets/");

    while (data_file.MoreKeys())
    {
        auto data = data_file.GetKey();
        if (data.key == "Sprite")
        {
            auto ss = data.ss;
            std::string path;

            *ss >> path;
            sprite_path += path;
        }
        else if (data.key == "Collisions")
        {
            auto ss = data.ss;

            *ss >> collisions;
        }
        else if (data.key == "Frames")
        {
            auto ss = data.ss;
            *ss >> config;
        }
        else if (data.key == "Behavior")
        {
            auto ss = data.ss;
            std::string identifier;
            *ss >> identifier;

            if (identifier == "None")
            {
                behavior = Behavior::None;
            }
            else if (identifier == "Spiny")
            {
                behavior = Behavior::Spiny;
            }
        }
        else if (data.key == "Damage")
        {
            auto ss = data.ss;
            *ss >> damage;
        }
        else if (data.key == "Knockback")
        {
            auto ss = data.ss;
            *ss >> knockback;
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

bool Enemy::HasCollisions()
{
    return collisions;
}
