#ifndef ENEMY_H
#define ENEMY_H

#include "player.h"

enum class Behavior
{
    None, Spiny, Lizard
};

class Enemy
{
public:
    Enemy(std::string identifier);

    void Update(sf::Time elapsed, sf::RenderWindow& window, Player& player);
    void Draw(sf::RenderWindow& window);

    void SetAnimation(std::string animation_name);
    void StopAnimation();
    sf::Sprite& GetSprite();
    bool HasCollisions();

private:
    void load(std::string filepath);
    void spinyUpdate(sf::Time elapsed, sf::RenderWindow& window, Player& player);
    void lizardUpdate(sf::Time elapsed, sf::RenderWindow& window, Player& player);

    sf::Vector2f follow(sf::Vector2f player_position, sf::Time elapsed);
    void wander(sf::Time elapsed);

    sf::Vector2f getPlayerDirection(sf::Vector2f player_position);

    Spritesheet sprite;
    sf::Vector2f velocity{0, 0};
    float movespeed;
    bool collisions;
    Behavior behavior;
    int damage;
    int knockback;
};

#endif // ENEMY_H
