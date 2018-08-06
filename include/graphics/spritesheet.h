/**************************************************************************************************
 *  File:       spritesheet.h
 *  Class:      Spritesheet
 *
 *  Purpose:    Spritesheet is a wrapper for an sf::Sprite that holds config information that
 *              describes the coordinates on the sheet of each frame.
 *
 *  Author:     Ryan Berge
 *
 *************************************************************************************************/
#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
#include <vector>

class Spritesheet
{
public:
    struct Config
    {
        std::vector<sf::IntRect> frames;
    };

    Spritesheet();
    Spritesheet(std::string filepath);
    Spritesheet(std::string filepath, Config config);
    Spritesheet(const Spritesheet& other);

    Spritesheet& operator=(const Spritesheet& other);

    void Draw(sf::RenderWindow& window);
    bool SetFrame(int frame);
    void SetConfig(Config config);

    sf::Sprite& GetSprite();

private:
    Config config;
    sf::Texture texture;
    sf::Sprite sprite;
    std::string texture_filepath;
    int frame;
    bool is_valid;

};

#endif // SPRITESHEET_H
