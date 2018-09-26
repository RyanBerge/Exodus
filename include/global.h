#ifndef GLOBAL_H
#define GLOBAL_H

#include <SFML/Graphics/Rect.hpp>
#include <functional>

class Global
{
public:
    static std::function<sf::Vector2f(sf::FloatRect, sf::Vector2f)> MovePlayer;
    static std::function<sf::Vector2f(sf::FloatRect, sf::Vector2f, int)> MoveEntity;
    static std::function<void(sf::Vector2i)> ChangeRoom;
    static std::function<void(void)> Death;
    static std::function<void(void)> Resume;
    static std::function<void(void)> Quit;
    static std::function<void(void)> QuitToMenu;
    static std::function<void(void)> Play;
    static std::function<void(bool)> Fullscreen;
};

#endif // GLOBAL_H