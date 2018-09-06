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
#include <SFML/Graphics/RenderTexture.hpp>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <memory>

class Spritesheet
{
public:
    struct Frame
    {
        sf::IntRect bounds;
        sf::Vector2f origin;
    };

    struct Config
    {
        std::vector<Frame> frames;
    };

    struct Animation
    {
        std::string name;
        int first_frame;
        int second_frame;
        float speed;
        int center_x;
        int center_y;
        std::string next_animation;
    };

    struct LightConfig
    {
        int x;
        int y;
        std::string identifier;
        std::string animation;
    };

    enum class Direction
    {
        Up, Down, Left, Right
    };

    Spritesheet();
    Spritesheet(std::string filepath);
    Spritesheet(std::string filepath, Config config);

    void Draw(sf::RenderWindow& window);
    void DrawLighting(sf::RenderTexture& target);
    void Update(sf::Time elapsed, sf::RenderWindow& window);

    bool SetFrame(int frame);
    void SetConfig(Config config);
    void AddLightFrame(int frame, LightConfig light_config);
    void AddAnimation(Animation animation);
    bool SetAnimation(std::string name);
    void StopAnimation();
    void AdvanceAnimation();
    void SetFlash(sf::Color, float duration, float rate);

    sf::Sprite& GetSprite();
    std::list<LightConfig> GetLights();
    std::string GetAnimation();

private:
    Config config{};
    std::shared_ptr<sf::Texture> texture{new sf::Texture()};
    sf::Sprite sprite{};
    std::string texture_filepath{""};

    std::map<std::string, Animation> animations{};
    std::string current_animation{""};
    std::vector<std::list<LightConfig>> light_configs{};
    float animation_time{0};
    int frame{-1};

    sf::Color flash_color{255, 255, 255};
    float flash_rate{0};
    float flash_timer{0};
    float flash_complete_timer{0};

    bool is_valid{false};
};

std::stringstream& operator>>(std::stringstream& ss, Spritesheet::Config& config);

#endif // SPRITESHEET_H
