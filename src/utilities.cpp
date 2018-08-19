#include "utilities.h"

#include <cmath>

namespace Utilities
{
    sf::Vector2f Normalize(sf::Vector2f vector)
    {
        float magnitude = sqrt(vector.x * vector.x + vector.y * vector.y);
        if (magnitude != 0)
        {
            return sf::Vector2f(vector.x / magnitude, vector.y / magnitude);
        }
        else
        {
            return vector;
        }
    }


    bool CheckCollision(sf::FloatRect left, sf::FloatRect right)
    {
        if (left.left < right.left + right.width && left.left + left.width > right.left &&
            left.top < right.top + right.height && left.top + left.height > right.top)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
