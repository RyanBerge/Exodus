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

    sf::Sprite& GetSprite();
    bool HasCollisions();

private:
    void load(std::string filepath);

    Spritesheet sprite;
    bool collisions{false};
};

#endif // ENTITY_H
