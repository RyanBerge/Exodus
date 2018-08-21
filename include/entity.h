#ifndef ENTITY_H
#define ENTITY_H

#include "graphics/spritesheet.h"

class Entity
{
public:
    Entity(std::string identifier);

    void Update(sf::Time elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

    sf::Sprite& GetSprite();
    bool HasCollisions();

private:
    void load(std::string filepath);

    Spritesheet sprite;
    bool collisions;
};

#endif // ENTITY_H
