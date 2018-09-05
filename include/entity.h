#ifndef ENTITY_H
#define ENTITY_H

#include "graphics/spritesheet.h"

class Entity
{
public:
    Entity(std::string identifier);

    void Update(sf::Time elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);
    void DrawLighting(sf::RenderTexture& target);

    void SetAnimation(std::string animation_name);
    float Collide(sf::Time elapsed);

    int GetId();
    std::string GetType();
    sf::Sprite& GetSprite();
    bool HasCollisions();

private:
    void load(std::string filepath);

    int id{0};
    std::string type{""};
    Spritesheet sprite;
    bool collisions{false};
    bool colliding{false};
    float collision_timer{0};
};

#endif // ENTITY_H
