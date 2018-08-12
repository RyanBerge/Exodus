#include "graphics/spritesheet.h"

#include <iostream>

Spritesheet::Spritesheet() : config{}, texture{}, sprite{}, animations{}, current_animation{""},
        animation_time{0}, texture_filepath{""}, frame{-1}, is_valid{false}
{
}

Spritesheet::Spritesheet(std::string filepath, Config config) : config{config}, texture{}, sprite{}, animations{}, current_animation{""},
        animation_time{0}, texture_filepath{filepath}, frame{-1}, is_valid{false}
{
    if (texture_filepath != "" && texture.loadFromFile(texture_filepath))
    {
        sprite.setTexture(texture);
        is_valid = true;
        SetFrame(0);
    }
}

Spritesheet::Spritesheet(const Spritesheet& other) : config{}, texture{}, sprite{}, animations{}, current_animation{""},
        animation_time{0}, texture_filepath{""}, frame{-1}, is_valid{false}
{
    *this = other;
}

Spritesheet::Spritesheet(std::string filepath) : config{}, texture{}, sprite{}, animations{}, current_animation{""},
        animation_time{0}, texture_filepath{filepath}, frame{-1}, is_valid{false}
{
    if (texture_filepath != "" && texture.loadFromFile(texture_filepath))
    {
        sprite.setTexture(texture);
        is_valid = true;
    }
}

void Spritesheet::Update(sf::Time elapsed, sf::RenderWindow& window)
{
    if (current_animation != "" && animations[current_animation].speed != 0)
    {
        animation_time += elapsed.asSeconds();
        if (animation_time >= animations[current_animation].speed)
        {
            animation_time = animation_time - animations[current_animation].speed;
            AdvanceAnimation();
        }
    }
}

void Spritesheet::Draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Spritesheet::SetConfig(Config config)
{
    this->config = config;
    SetFrame(0);
}

void Spritesheet::AddAnimation(Animation animation)
{
    animations[animation.name] = animation;
}

bool Spritesheet::SetAnimation(std::string name)
{
    if (animations.find(name) != animations.end())
    {
        current_animation = name;
        SetFrame(animations[name].first_frame);
        animation_time = 0;
        return true;
    }
    else
    {
        return false;
    }
}

void Spritesheet::AdvanceAnimation()
{
    if (current_animation != "")
    {
        if (frame == animations[current_animation].second_frame)
        {
            SetFrame(animations[current_animation].first_frame);
        }
        else
        {
            SetFrame(frame + 1);
        }
    }
}

Spritesheet& Spritesheet::operator=(const Spritesheet& other)
{
    config = other.config;
    sprite = other.sprite;
    animations = other.animations;
    current_animation = other.current_animation;
    animation_time = other.animation_time;
    texture_filepath = other.texture_filepath;
    if (texture_filepath != "" && texture.loadFromFile(texture_filepath))
    {
        sprite.setTexture(texture);
        is_valid = true;
        SetFrame(other.frame);
    }
    return *this;
}

bool Spritesheet::SetFrame(int new_frame)
{
    if (!is_valid)
    {
        return false;
    }

    if (static_cast<int>(config.frames.size()) > 0 && static_cast<int>(config.frames.size()) > new_frame)
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
