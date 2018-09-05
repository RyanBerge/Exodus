#ifndef ENTITY_H
#define ENTITY_H

#include "graphics/spritesheet.h"

class Entity
{
public:
    Entity(std::string identifier);

    void Update(sf::Time elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);
    void DrawLights(sf::RenderTexture& target);

    void SetAnimation(std::string animation_name);
    float Collide(sf::Time elapsed);

    int GetId();
    std::string GetType();
    sf::Sprite& GetSprite();
    std::string GetAnimation();
    std::list<Entity> GetLights();
    bool HasCollisions();

private:
    void load(std::string filepath);
    void drawLighting(sf::RenderTexture& target);

    int id{0};
    std::string type{""};
    Spritesheet sprite{};
    std::list<Entity> lights{};
    bool collisions{false};
    bool colliding{false};
    float collision_timer{0};
};

#endif // ENTITY_H
