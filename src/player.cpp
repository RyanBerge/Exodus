#include "player.h"
#include <SFML/Window/Keyboard.hpp>

#include <iostream>

#define MOVESPEED 125

Player::Player() : sprite{Spritesheet("assets/Player.png")}, direction{Spritesheet::Direction::Down}
{
    Spritesheet::Config config;
    config.frames.push_back(sf::IntRect(0, 0, 50, 50));
    config.frames.push_back(sf::IntRect(50, 0, 50, 50));
    config.frames.push_back(sf::IntRect(100, 0, 50, 50));

    config.frames.push_back(sf::IntRect(0, 50, 50, 50));
    config.frames.push_back(sf::IntRect(50, 50, 50, 50));
    config.frames.push_back(sf::IntRect(100, 50, 50, 50));

    config.frames.push_back(sf::IntRect(0, 100, 50, 50));
    config.frames.push_back(sf::IntRect(50, 100, 50, 50));

    config.frames.push_back(sf::IntRect(0, 150, 50, 50));
    config.frames.push_back(sf::IntRect(50, 150, 50, 50));

    sprite.SetConfig(config);

    sprite.AddAnimation(Spritesheet::Animation{"IdleDown", 0, 0, 0});
    sprite.AddAnimation(Spritesheet::Animation{"IdleUp", 3, 3, 0});
    sprite.AddAnimation(Spritesheet::Animation{"IdleLeft", 6, 6, 0});
    sprite.AddAnimation(Spritesheet::Animation{"IdleRight", 8, 8, 0});
    sprite.AddAnimation(Spritesheet::Animation{"WalkDown", 1, 2, 2.0/5});
    sprite.AddAnimation(Spritesheet::Animation{"WalkUp", 4, 5, 2.0/5});
    sprite.AddAnimation(Spritesheet::Animation{"WalkLeft", 6, 7, 1.0/5});
    sprite.AddAnimation(Spritesheet::Animation{"WalkRight", 8, 9, 1.0/5});

    sprite.SetAnimation("IdleDown");
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

    sprite.GetSprite().move(velocity);

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

