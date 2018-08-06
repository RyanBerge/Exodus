#include "graphics/spritesheet.h"

Spritesheet::Spritesheet() : config{}, texture{}, sprite{}, texture_filepath{""}, frame{-1}, is_valid{false}
{
}

Spritesheet::Spritesheet(std::string filepath, Config config) : config{config}, texture{},
        sprite{}, texture_filepath{filepath}, frame{-1}, is_valid{false}
{
    if (texture.loadFromFile(texture_filepath))
    {
        sprite.setTexture(texture);
        is_valid = true;
        SetFrame(0);
    }
}

Spritesheet::Spritesheet(const Spritesheet& other) : config{}, texture{}, sprite{}, texture_filepath{""}, frame{-1}, is_valid{false}
{
    *this = Spritesheet(other);
}

Spritesheet& Spritesheet::operator=(const Spritesheet& other)
{
    config = other.config;
    texture_filepath = other.texture_filepath;
    if (texture.loadFromFile(texture_filepath))
    {
        sprite.setTexture(texture);
        is_valid = true;
        SetFrame(0);
    }
    return *this;
}

void Spritesheet::Draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

bool Spritesheet::SetFrame(int new_frame)
{
    if (!is_valid)
    {
        return false;
    }

    if (static_cast<int>(config.frames.size()) > new_frame)
    {
        frame = new_frame;
        sprite.setTextureRect(config.frames[new_frame]);
    }
    else
    {
        return false;
    }

    return true;
}

sf::Sprite& Spritesheet::GetSprite()
{
    return sprite;
}
