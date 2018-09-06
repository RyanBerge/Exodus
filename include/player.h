#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "hud.h"
#include <functional>

class Player
{
public:
    Player();

    void Update(sf::Time elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

    void Damage(int damage, int knockback, sf::Vector2f direction);

    sf::Sprite& GetSprite();
    Spritesheet::Direction GetDirection();
    void SetHudViewport(sf::FloatRect viewport);

    void RegisterCollisionCheck(std::function<bool(sf::IntRect)> f);
    void RegisterChangeRoom(std::function<void(sf::Vector2i)> f);
    void RegisterDeathCallback(std::function<void(void)> f);

private:
    void load(std::string filepath);
    void determineMovement(sf::Time elapsed);

    std::function<bool(sf::IntRect)> checkCollisions;
    std::function<void(sf::Vector2i)> changeRoom;
    std::function<void(void)> deathCallback;

    Spritesheet sprite;
    Spritesheet::Direction direction;
    sf::Vector2f velocity;

    int health{5};
    int movespeed{0};
    float knockback_magnitude{0};
    sf::Vector2f knockback_direction{0, 0};
    float knockback_decay{0};
    bool invincible{false};
    float invincible_timer{0};
    Hud hud;

};

#endif // PLAYER_H
