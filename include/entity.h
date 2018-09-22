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

    void Update(sf::Time elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);
    void DrawLights(sf::RenderTexture& target);

    void SetAnimation(std::string animation_name);
    Collision Collide(sf::Time elapsed, Player& player);
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

private:
    void load(std::string filepath);
    void drawLighting(sf::RenderTexture& target);

    int id{0};
    std::string label{""};
    std::string type{""};
    Spritesheet sprite{};
    std::list<Entity> lights{};
    std::list<Trigger> triggers{};
    bool collisions{false};
    bool colliding{false};
    float collision_timer{0};
    int damage{0};
    int knockback{0};
};

#endif // ENTITY_H
