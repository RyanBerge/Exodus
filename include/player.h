#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "graphics/spritesheet.h"
#include <functional>

class Player
{
public:
    Player();

    void Update(sf::Time elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

    sf::Sprite& GetSprite();
    void RegisterCollisionCheck(std::function<bool(sf::IntRect)> f);
    void RegisterChangeRoom(std::function<void(sf::Vector2i)> f);

private:
    void load(std::string filepath);
    void determineMovement(sf::Time elapsed);

    std::function<bool(sf::IntRect)> checkCollisions;
    std::function<void(sf::Vector2i)> changeRoom;

    Spritesheet sprite;
    Spritesheet::Direction direction;
    sf::Vector2f velocity;

};

#endif // PLAYER_H
