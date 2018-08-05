#include "graphics/spritesheet.h"

Spritesheet::Spritesheet() : config{}, texture{}, sprite{}, frame{-1}, is_valid{false}
{
}

Spritesheet::Spritesheet(std::string filepath, Config config) : config{config}, texture{},
        sprite{}, frame{-1}, is_valid{false}
{
    if (texture.loadFromFile(filepath))
    {
        sprite.setTexture(texture);
        is_valid = true;
    }
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
