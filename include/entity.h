#ifndef ENTITY_H
#define ENTITY_H

#include "graphics/spritesheet.h"
#include "player.h"

struct Collision
{
    bool blocking;
    float collision_timer;
    int damage;
    int knockback;
};

struct Trigger
{
    std::string callback_key;
    std::string type;
};

class Entity
{
public:
    Entity(std::string identifier, std::string label);

    void Update(sf::Time elapsed, sf::RenderWindow& window, Player& player);
    void Draw(sf::RenderWindow& window);
    void DrawLights(sf::RenderTexture& target);

    void SetAnimation(std::string animation_name);
    Collision Collide(sf::Time elapsed, Player& player);
    Collision EntityCollide(sf::Time elapsed, Entity& other);
    sf::Vector2f GetKnockbackDirection(Player& player);

    int GetId();
    std::string GetType();
    sf::Sprite& GetSprite();
    std::string GetAnimation();
    std::string GetLabel();
    std::list<Entity>& GetLights();
    std::list<Trigger>& GetTriggers();
    bool HasCollisions();
    int GetDamage();
    int GetKnockback();
    sf::FloatRect GetHitbox();
    bool IsDead();
    void Kill();

    bool SetBurning();

private:
    void load(std::string filepath);
    void drawLighting(sf::RenderTexture& target);

    void spinyUpdate(sf::Time elapsed, sf::RenderWindow& window, Player& player);
    void lizardUpdate(sf::Time elapsed, sf::RenderWindow& window, Player& player);

    sf::Vector2f follow(sf::Vector2f player_position, sf::Time elapsed);
    void wander(sf::Time elapsed);

    sf::Vector2f getPlayerDirection(sf::Vector2f player_position);

    int id{0};
    std::string label{""};
    std::string type{""};
    Spritesheet sprite{};
    std::list<Entity> lights{};
    std::list<Trigger> triggers{};
    bool collisions{false};
    bool entity_collisions{false};
    bool colliding{false};
    float collision_timer{0};
    sf::Vector2f velocity{0, 0};
    float movespeed;
    int damage{0};
    int health{1};
    int knockback{0};
    std::string behavior;
    bool is_burning{false};
    float burn_duration{0};
    bool dead{false};
};

#endif // ENTITY_H
