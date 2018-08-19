#ifndef UTILITES_H
#define UTILITES_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace Utilities
{
    sf::Vector2f Normalize(sf::Vector2f vector);
    bool CheckCollision(sf::FloatRect left, sf::FloatRect right);
}

#endif // UTILITIES_H