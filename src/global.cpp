#include "global.h"

std::function<sf::Vector2f(sf::FloatRect, sf::Vector2f)> Global::MovePlayer;
std::function<sf::Vector2f(sf::FloatRect, sf::Vector2f, Entity&)> Global::MoveEntity;
std::function<void(sf::Vector2i)> Global::ChangeRoom;
std::function<void(void)> Global::Death;
std::function<void(void)> Global::Resume;
std::function<void(void)> Global::Quit;
std::function<void(void)> Global::QuitToMenu;
std::function<void(void)> Global::Play;
std::function<void(bool)> Global::Fullscreen;
