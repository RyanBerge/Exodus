#include "player.h"
#include <SFML/Window/Keyboard.hpp>
#include <fstream>
#include <sstream>

#include <iostream>

#define MOVESPEED 125

Player::Player() : sprite{Spritesheet("assets/Player.png")}, direction{Spritesheet::Direction::Down}
{
    load("data/player/animations.txt");
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

    // Knockback
    velocity.x = knockback_direction.x * knockback_magnitude * elapsed.asSeconds();
    velocity.y = knockback_direction.y * knockback_magnitude * elapsed.asSeconds();

    knockback_magnitude -= knockback_decay * elapsed.asSeconds();
    if (knockback_magnitude <= 0)
    {
        knockback_magnitude = 0;
        knockback_direction.x = 0;
        knockback_direction.y = 0;
        knockback_decay = 0;
    }

    velocity.x += horizontal_direction * MOVESPEED * elapsed.asSeconds();
    velocity.y += vertical_direction * MOVESPEED * elapsed.asSeconds();

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
    else
    {
        // float min = 0;
        // float max = velocity.x;
        // float distance = velocity.x;

        // while (abs(min - max) > 1)
        // {
        //     distance = (min + max) / 2;
        //     new_position.left = sprite.GetSprite().getPosition().x + distance;
        //     if (checkCollisions(new_position))
        //     {
        //         max = distance;
        //     }
        //     else
        //     {
        //         min = distance;
        //     }
        // }

        // new_position.left = sprite.GetSprite().getPosition().x + distance;
        // if (!checkCollisions(new_position))
        // {
        //     sprite.GetSprite().move(distance, 0);
        // }
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
    else
    {
        // float min = 0;
        // float max = velocity.y;
        // float distance = velocity.y;

        // while (abs(min - max) > 1)
        // {
        //     distance = (min + max) / 2;
        //     new_position.top = sprite.GetSprite().getPosition().y + distance;
        //     if (checkCollisions(new_position))
        //     {
        //         max = distance;
        //     }
        //     else
        //     {
        //         min = distance;
        //     }
        // }

        // new_position.top = sprite.GetSprite().getPosition().y + distance;
        // if (!checkCollisions(new_position))
        // {
        //     sprite.GetSprite().move(0, distance);
        // }
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
    if (sprite.GetSprite().getPosition().x < 0 - sprite.GetSprite().getGlobalBounds().width)
    {
        changeRoom(sf::Vector2i(-1, 0));
    }
    else if (sprite.GetSprite().getPosition().y < 0 - sprite.GetSprite().getGlobalBounds().height)
    {
        changeRoom(sf::Vector2i(0, -1));
    }
    else if (sprite.GetSprite().getPosition().x > 1200)
    {
        changeRoom(sf::Vector2i(1, 0));
    }
    else if (sprite.GetSprite().getPosition().y > 800)
    {
        changeRoom(sf::Vector2i(0, 1));
    }

    if (invincible)
    {
        invincible_timer -= elapsed.asSeconds();
        if (invincible_timer <= 0)
        {
            invincible = false;
            invincible_timer = 0;
        }
    }
}

void Player::Draw(sf::RenderWindow& window)
{
    sprite.Draw(window);
}

void Player::Damage(int damage, int knockback, sf::Vector2f direction)
{
    if (!invincible)
    {
        health -= damage;
        if (health <= 0)
        {
            //TODO: die
        }

        sprite.SetFlash(sf::Color(255, 0, 0), 0.5, 10);

        knockback_magnitude = knockback;
        knockback_direction = direction;
        knockback_decay = knockback * 5;
        invincible = true;
        invincible_timer = 0.5;
    }
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

void Player::RegisterChangeRoom(std::function<void(sf::Vector2i)> f)
{
    changeRoom = f;
}
