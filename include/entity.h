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

private:
    Spritesheet sprite;
};

#endif // ENTITY_H
