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
    bool Heal(int health);

    sf::Sprite& GetSprite();
    sf::FloatRect GetHitbox();
    Spritesheet::Direction GetDirection();
    void SetHudViewport(sf::FloatRect viewport);
    void SetFrozen(bool frozen);
    void SetAnimation(std::string animation);

private:
    void load(std::string filepath);
    void determineMovement(sf::Time elapsed);

    Spritesheet sprite;
    Spritesheet::Direction direction;
    sf::Vector2f velocity;

    int max_health{5};
    int health{max_health};
    int movespeed{0};
    float knockback_magnitude{0};
    sf::Vector2f knockback_direction{0, 0};
    float knockback_decay{0};
    bool invincible{false};
    bool frozen{false};
    float invincible_timer{0};
    Hud hud;

};

#endif // PLAYER_H
