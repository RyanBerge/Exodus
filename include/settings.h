#ifndef SETTINGS_H
#define SETTINGS_H

#include <SFML/System/Vector2.hpp>
#include <string>

class Settings
{
public:
    static void LoadSettings(std::string filepath);

    static sf::Vector2u video_resolution;
};

#endif // SETTINGS_H
