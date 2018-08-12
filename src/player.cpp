#include "player.h"
#include <SFML/Window/Keyboard.hpp>
#include <fstream>
#include <sstream>

#include <iostream>

#define MOVESPEED 125

Player::Player() : sprite{Spritesheet("assets/Player.png")}, direction{Spritesheet::Direction::Down}
{
    load("data/player/animations.txt");
    // Spritesheet::Config config;
    // config.frames.push_back(sf::IntRect(0, 0, 50, 50));
    // config.frames.push_back(sf::IntRect(50, 0, 50, 50));
    // config.frames.push_back(sf::IntRect(100, 0, 50, 50));

    // config.frames.push_back(sf::IntRect(0, 50, 50, 50));
    // config.frames.push_back(sf::IntRect(50, 50, 50, 50));
    // config.frames.push_back(sf::IntRect(100, 50, 50, 50));

    // config.frames.push_back(sf::IntRect(0, 100, 50, 50));
    // config.frames.push_back(sf::IntRect(50, 100, 50, 50));

    // config.frames.push_back(sf::IntRect(0, 150, 50, 50));
    // config.frames.push_back(sf::IntRect(50, 150, 50, 50));

    // sprite.SetConfig(config);

    // sprite.AddAnimation(Spritesheet::Animation{"IdleDown", 0, 0, 0});
    // sprite.AddAnimation(Spritesheet::Animation{"IdleUp", 3, 3, 0});
    // sprite.AddAnimation(Spritesheet::Animation{"IdleLeft", 6, 6, 0});
    // sprite.AddAnimation(Spritesheet::Animation{"IdleRight", 8, 8, 0});
    // sprite.AddAnimation(Spritesheet::Animation{"WalkDown", 1, 2, 2.0/5});
    // sprite.AddAnimation(Spritesheet::Animation{"WalkUp", 4, 5, 2.0/5});
    // sprite.AddAnimation(Spritesheet::Animation{"WalkLeft", 6, 7, 1.0/5});
    // sprite.AddAnimation(Spritesheet::Animation{"WalkRight", 8, 9, 1.0/5});

    // sprite.SetAnimation("IdleDown");
}

void Player::determineMovement(sf::Time elapsed)
{
    int horizontal_direction = 0;
    int vertical_direction = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        horizontal_direction -= 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        horizontal_direction += 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        vertical_direction -= 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        vertical_direction += 1;
    }

    sf::Vector2f old_velocity(velocity);

    velocity.x = horizontal_direction * MOVESPEED * elapsed.asSeconds();
    velocity.y = vertical_direction * MOVESPEED * elapsed.asSeconds();

    sf::IntRect new_position(sprite.GetSprite().getGlobalBounds());
    new_position.left = sprite.GetSprite().getPosition().x + velocity.x;
    new_position.top = sprite.GetSprite().getPosition().y;

    if (!checkCollisions(new_position))
    {
        sprite.GetSprite().move(velocity.x, 0);
    }
    else if (!checkCollisions(sf::IntRect(new_position.left, new_position.top + velocity.x, new_position.width, new_position.height)))
    {
        sprite.GetSprite().move(velocity.x, velocity.x);
    }
    else if (!checkCollisions(sf::IntRect(new_position.left, new_position.top - velocity.x, new_position.width, new_position.height)))
    {
        sprite.GetSprite().move(velocity.x, velocity.x * -1);
    }

    new_position = sf::IntRect(sprite.GetSprite().getGlobalBounds());
    new_position.left = sprite.GetSprite().getPosition().x;
    new_position.top = sprite.GetSprite().getPosition().y + velocity.y;

    if (!checkCollisions(new_position))
    {
        sprite.GetSprite().move(0, velocity.y);
    }
    else if (!checkCollisions(sf::IntRect(new_position.left + velocity.y, new_position.top, new_position.width, new_position.height)))
    {
        sprite.GetSprite().move(velocity.y, velocity.y);
    }
    else if (!checkCollisions(sf::IntRect(new_position.left - velocity.y, new_position.top, new_position.width, new_position.height)))
    {
        sprite.GetSprite().move(velocity.y * -1, velocity.y);
    }

    Spritesheet::Direction old_direction = direction;

    if (horizontal_direction > 0)
    {
        if (old_direction != Spritesheet::Direction::Right || (old_velocity.x == 0 && old_velocity.y == 0))
        {
            direction = Spritesheet::Direction::Right;
            sprite.SetAnimation("WalkRight");
        }
    }
    else if (horizontal_direction < 0)
    {
        if (old_direction != Spritesheet::Direction::Left || (old_velocity.x == 0 && old_velocity.y == 0))
        {
            direction = Spritesheet::Direction::Left;
            sprite.SetAnimation("WalkLeft");
        }
    }
    else if (vertical_direction > 0)
    {
        if (old_direction != Spritesheet::Direction::Down || (old_velocity.x == 0 && old_velocity.y == 0))
        {
            direction = Spritesheet::Direction::Down;
            sprite.SetAnimation("WalkDown");
        }
    }
    else if (vertical_direction < 0)
    {
        if (old_direction != Spritesheet::Direction::Up || (old_velocity.x == 0 && old_velocity.y == 0))
        {
            direction = Spritesheet::Direction::Up;
            sprite.SetAnimation("WalkUp");
        }
    }

    if (horizontal_direction == 0 && vertical_direction == 0)
    {
        switch (direction)
        {
            case Spritesheet::Direction::Down:
            {
                sprite.SetAnimation("IdleDown");
            }
            break;
            case Spritesheet::Direction::Up:
            {
                sprite.SetAnimation("IdleUp");
            }
            break;
            case Spritesheet::Direction::Left:
            {
                sprite.SetAnimation("IdleLeft");
            }
            break;
            case Spritesheet::Direction::Right:
            {
                sprite.SetAnimation("IdleRight");
            }
            break;
        }
    }
}

void Player::Update(sf::Time elapsed, sf::RenderWindow& window)
{
    determineMovement(elapsed);
    sprite.Update(elapsed, window);
}

void Player::Draw(sf::RenderWindow& window)
{
    sprite.Draw(window);
}

sf::Sprite& Player::GetSprite()
{
    return sprite.GetSprite();
}

void Player::load(std::string filepath)
{
    std::ifstream file(filepath);
    std::string line;

    std::string sprite_path("assets/Player.png");
    Spritesheet::Config config;

    sprite = Spritesheet(sprite_path);

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

        if (key == "Frames")
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
            sprite.SetConfig(config);
        }
        else
        {
            std::string animation_name = key;
            std::stringstream ss(std::string(sit, line.end()));
            int start_frame;
            int end_frame;
            float animation_speed;
            ss >> start_frame;
            ss >> end_frame;
            ss >> animation_speed;
            sprite.AddAnimation(Spritesheet::Animation{animation_name, start_frame, end_frame, animation_speed});
        }
    }
}

void Player::RegisterCollisionCheck(std::function<bool(sf::IntRect)> f)
{
    checkCollisions = f;
}
