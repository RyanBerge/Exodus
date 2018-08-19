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
#include <map>
#include <memory>

class Spritesheet
{
public:
    struct Config
    {
        std::vector<sf::IntRect> frames;
    };

    struct Animation
    {
        std::string name;
        int first_frame;
        int second_frame;
        float speed;
    };

    enum class Direction
    {
        Up, Down, Left, Right
    };

    Spritesheet();
    Spritesheet(std::string filepath);
    Spritesheet(std::string filepath, Config config);

    void Draw(sf::RenderWindow& window);
    void Update(sf::Time elapsed, sf::RenderWindow& window);

    bool SetFrame(int frame);
    void SetConfig(Config config);
    void AddAnimation(Animation animation);
    bool SetAnimation(std::string name);
    void AdvanceAnimation();
    void SetFlash(sf::Color, float duration, float rate);

    sf::Sprite& GetSprite();

private:
    Config config;
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
    std::map<std::string, Animation> animations;
    std::string current_animation;
    float animation_time;
    std::string texture_filepath;
    int frame;
    bool is_valid;
    sf::Color flash_color{255, 255, 255};
    float flash_rate{0};
    float flash_timer{0};
    float flash_complete_timer{0};
};

#endif // SPRITESHEET_H
