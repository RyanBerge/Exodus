#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include "graphics/spritesheet.h"

class Player
{
public:
    Player();

    void Update(sf::Time elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

    sf::Sprite& GetSprite();

private:
    void determineMovement(sf::Time elapsed);

    Spritesheet sprite;
    Spritesheet::Direction direction;
    sf::Vector2f velocity;

};

#endif // PLAYER_H
