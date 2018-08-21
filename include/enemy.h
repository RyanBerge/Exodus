#ifndef ENEMY_H
#define ENEMY_H

#include "player.h"

enum class Behavior
{
    None, Spiny
};

class Enemy
{
public:
    Enemy(std::string identifier);

    void Update(sf::Time elapsed, sf::RenderWindow& window, Player& player);
    void Draw(sf::RenderWindow& window);

    sf::Sprite& GetSprite();
    bool HasCollisions();

private:
    void load(std::string filepath);
    void spinyUpdate(sf::Time elapsed, sf::RenderWindow& window, Player& player);
    sf::Vector2f follow(sf::Vector2f player_position, sf::Time elapsed);

    Spritesheet sprite;
    bool collisions;
    Behavior behavior;
    int damage;
    int knockback;
};

#endif // ENEMY_H
