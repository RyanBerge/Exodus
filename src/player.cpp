#include "player.h"
#include <SFML/Window/Keyboard.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

#include "data_file.h"
#include "utilities.h"
#include "global.h"

Player::Player() : sprite{Spritesheet("assets/Player.png")}, direction{Spritesheet::Direction::Down}
{
    load("data/player/player.txt");
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

    velocity.x += horizontal_direction * movespeed * elapsed.asSeconds();
    velocity.y += vertical_direction * movespeed * elapsed.asSeconds();

    velocity = Global::MovePlayer(sprite.GetHitbox(), velocity);
    sprite.GetSprite().move(velocity);

    if (horizontal_direction > 0)
    {
        if (sprite.GetAnimation() != "WalkRight")
        {
            direction = Spritesheet::Direction::Right;
            sprite.SetAnimation("WalkRight");
        }
    }
    else if (horizontal_direction < 0)
    {
        if (sprite.GetAnimation() != "WalkLeft")
        {
            direction = Spritesheet::Direction::Left;
            sprite.SetAnimation("WalkLeft");
        }
    }
    else if (vertical_direction > 0)
    {
        if (sprite.GetAnimation() != "WalkDown")
        {
            direction = Spritesheet::Direction::Down;
            sprite.SetAnimation("WalkDown");
        }
    }
    else if (vertical_direction < 0)
    {
        if (sprite.GetAnimation() != "WalkUp")
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
    if (!frozen)
    {
        determineMovement(elapsed);
    }

    sprite.Update(elapsed, window);
    hud.Update(elapsed, window);

    if (sprite.GetSprite().getPosition().x < 0 - sprite.GetSprite().getGlobalBounds().width)
    {
        Global::ChangeRoom(sf::Vector2i(-1, 0));
    }
    else if (sprite.GetSprite().getPosition().y < 0 - sprite.GetSprite().getGlobalBounds().height)
    {
        Global::ChangeRoom(sf::Vector2i(0, -1));
    }
    else if (sprite.GetSprite().getPosition().x > 1200)
    {
        Global::ChangeRoom(sf::Vector2i(1, 0));
    }
    else if (sprite.GetSprite().getPosition().y > 800)
    {
        Global::ChangeRoom(sf::Vector2i(0, 1));
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
    hud.Draw(window);
}

void Player::Damage(int damage, int knockback, sf::Vector2f direction)
{
    if (!invincible)
    {
        health -= damage;
        hud.SetHealth(health);
        if (health <= 0)
        {
            Global::Death();
            return;
        }

        sprite.SetFlash(sf::Color(255, 0, 0), 0.5, 10);

        knockback_magnitude = knockback;
        knockback_direction = direction;
        knockback_decay = knockback * 5;
        invincible = true;
        invincible_timer = 0.5;
    }
}

bool Player::Heal(int amount)
{
    if (health == max_health)
    {
        return false;
    }

    health += amount;
    if (health > max_health)
    {
        health = max_health;
    }

    hud.SetHealth(health);
    return true;
}

sf::Sprite& Player::GetSprite()
{
    return sprite.GetSprite();
}

Spritesheet::Direction Player::GetDirection()
{
    return direction;
}

sf::FloatRect Player::GetHitbox()
{
    return sprite.GetHitbox();
}

void Player::load(std::string filepath)
{
    std::string sprite_path("assets/");
    Spritesheet::Config config;
    std::vector<Spritesheet::Animation> animations;

    DataFile data_file;
    if (!data_file.Open(filepath))
    {
        std::cerr << "Exodus: Player could not be loaded: " << filepath << std::endl;
        return;
    }

    while (data_file.MoreKeys())
    {
        auto data = data_file.GetKey();
        if (data.key == "Sprite")
        {
            auto ss = data.ss;
            std::string path;
            *ss >> path;
            sprite_path += std::string(path);
        }
        if (data.key == "Movespeed")
        {
            auto ss = data.ss;
            *ss >> movespeed;
        }
        else if (data.key == "Frames")
        {
            auto ss = data.ss;
            *ss >> config;
        }
        else if (data.key == "Animation")
        {
            auto ss = data.ss;
            animations.push_back(Utilities::ReadAnimation(*ss));
        }
    }

    sprite = Spritesheet(sprite_path, config);
    for (auto animation : animations)
    {
        sprite.AddAnimation(animation);
    }
}

void Player::SetHudViewport(sf::FloatRect viewport)
{
    hud.SetViewport(viewport);
}

void Player::SetFrozen(bool frozen)
{
    this->frozen = frozen;
}

void Player::SetAnimation(std::string animation)
{
    sprite.SetAnimation(animation);
}
