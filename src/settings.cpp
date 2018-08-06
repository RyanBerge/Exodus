#include "settings.h"

sf::Vector2u Settings::video_resolution;

void Settings::LoadSettings(std::string filepath)
{
    // TODO: this function
    Settings::video_resolution = sf::Vector2u(1280, 800);
}
