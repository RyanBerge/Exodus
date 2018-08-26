#include "enemy.h"

#include "utilities.h"
#include "data_file.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

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
    std::vector<Spritesheet::Animation> animations;

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
        else if (data.key == "Animation")
        {
            auto ss = data.ss;
            int start_frame;
            int end_frame;
            int center_x = 0;
            int center_y = 0;
            float animation_speed;
            std::string animation_name;
            *ss >> animation_name;
            *ss >> start_frame;
            *ss >> end_frame;
            *ss >> animation_speed;
            *ss >> center_x;
            *ss >> center_y;

            animations.push_back(Spritesheet::Animation{animation_name, start_frame, end_frame, animation_speed, center_x, center_y});
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
            else if (identifier == "Lizard")
            {
                behavior = Behavior::Lizard;
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
        else if (data.key == "Movespeed")
        {
            auto ss = data.ss;
            *ss >> movespeed;
        }
    }

    sprite = Spritesheet(sprite_path, config);
    for (auto animation : animations)
    {
        sprite.AddAnimation(animation);
    }
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
        case Behavior::Lizard:
        {
            lizardUpdate(elapsed, window, player);
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

void Enemy::lizardUpdate(sf::Time elapsed, sf::RenderWindow& window, Player& player)
{
    wander(elapsed);
    sprite.GetSprite().move(velocity * elapsed.asSeconds());
    auto direction = getPlayerDirection(player.GetSprite().getPosition());
    if (Utilities::CheckCollision(sprite.GetSprite().getGlobalBounds(), player.GetSprite().getGlobalBounds()))
    {
        player.Damage(damage, knockback, direction);
    }
}

sf::Vector2f Enemy::follow(sf::Vector2f player_position, sf::Time elapsed)
{
    auto direction = getPlayerDirection(player_position);
    sprite.GetSprite().move(direction.x * movespeed * elapsed.asSeconds(), direction.y * movespeed * elapsed.asSeconds());
    return direction;
}

void Enemy::wander(sf::Time elapsed)
{
    static int direction = 0; // None
    static bool moving = false;
    static float behavior_distance = 0;
    static float current_distance = 0;

    if (direction == 0)
    {
        direction = (rand() % 4) + 1;
        behavior_distance = (rand() % 50) + 50;
    }

    current_distance += elapsed.asSeconds() * movespeed;
    if (current_distance >= behavior_distance)
    {
        if (moving)
        {
            moving = false;
            behavior_distance = (rand() % 50) + 50;
        }
        else
        {
            moving = true;
            direction = (rand() % 4) + 1;
            behavior_distance = (rand() % 50) + 50;
        }
        current_distance = 0;

        switch (direction)
        {
            case 1:
            {
                if (moving)
                {
                    sprite.SetAnimation("Left");
                    velocity = {-movespeed, 0};
                }
                else
                {
                    sprite.StopAnimation();
                    velocity = {0, 0};
                }
            }
            break;
            case 2:
            {
                if (moving)
                {
                    sprite.SetAnimation("Right");
                    velocity = {movespeed, 0};
                }
                else
                {
                    sprite.StopAnimation();
                    velocity = {0, 0};
                }
            }
            break;
            case 3:
            {
                if (moving)
                {
                    sprite.SetAnimation("Up");
                    velocity = {0, -movespeed};
                }
                else
                {
                    sprite.StopAnimation();
                    velocity = {0, 0};
                }
            }
            break;
            case 4:
            {
                if (moving)
                {
                    sprite.SetAnimation("Down");
                    velocity = {0, movespeed};
                }
                else
                {
                    sprite.StopAnimation();
                    velocity = {0, 0};
                }
            }
            break;
        }
    }
}

sf::Vector2f Enemy::getPlayerDirection(sf::Vector2f player_position)
{
    sf::Vector2f position = sprite.GetSprite().getPosition();
    sf::Vector2f direction = player_position - position;
    return Utilities::Normalize(direction);
}

void Enemy::Draw(sf::RenderWindow& window)
{
    sprite.Draw(window);
}

void Enemy::SetAnimation(std::string animation_name)
{
    sprite.SetAnimation(animation_name);
}

void Enemy::StopAnimation()
{
    sprite.StopAnimation();
}

sf::Sprite& Enemy::GetSprite()
{
    return sprite.GetSprite();
}

bool Enemy::HasCollisions()
{
    return collisions;
}
