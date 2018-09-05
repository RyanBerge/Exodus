#include "graphics/spritesheet.h"

#include <iostream>
#include <sstream>

Spritesheet::Spritesheet() : config{}, texture{new sf::Texture()}, sprite{}, animations{}, current_animation{""},
        animation_time{0}, texture_filepath{""}, frame{-1}, is_valid{false}
{
}

Spritesheet::Spritesheet(std::string filepath, Config config) : config{config}, texture{new sf::Texture()}, sprite{}, animations{}, current_animation{""},
        animation_time{0}, texture_filepath{filepath}, frame{-1}, is_valid{false}
{
    if (texture_filepath != "" && texture->loadFromFile(texture_filepath))
    {
        sprite.setTexture(*texture);
        is_valid = true;
        animations["Default"] = Spritesheet::Animation{"Default", 0, 0, 0, 0, 0};
    }
}

Spritesheet::Spritesheet(std::string filepath) : Spritesheet{filepath, Config{}}
{
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

    if (flash_color != sf::Color::White)
    {
        flash_timer -= elapsed.asSeconds();
        if (flash_timer <= 0)
        {
            if (sprite.getColor() == flash_color)
            {
                sprite.setColor(sf::Color::White);
            }
            else
            {
                sprite.setColor(flash_color);
            }
            flash_timer = flash_rate;
        }

        flash_complete_timer -= elapsed.asSeconds();
        if (flash_complete_timer <= 0)
        {
            flash_color = sf::Color::White;
            sprite.setColor(flash_color);
            flash_complete_timer = 0;
            flash_timer = 0;
            flash_rate = 0;
        }
    }
}

void Spritesheet::Draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Spritesheet::DrawLighting(sf::RenderTexture& target)
{
    sf::BlendMode mode(sf::BlendMode::SrcAlpha, sf::BlendMode::DstAlpha, sf::BlendMode::ReverseSubtract);
    target.draw(sprite, mode);
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
    if (name == "Random")
    {
        auto item = animations.begin();
        std::advance(item, rand() % animations.size());
        return SetAnimation(item->first);
    }
    else if (animations.find(name) != animations.end())
    {
        current_animation = name;
        SetFrame(animations[name].first_frame);
        animation_time = 0;
        sprite.setOrigin(animations[name].center_x, animations[name].center_y);
        return true;
    }
    else
    {
        return false;
    }
}

void Spritesheet::StopAnimation()
{
    current_animation = "";
}

void Spritesheet::AdvanceAnimation()
{
    if (current_animation != "")
    {
        if (frame == animations[current_animation].second_frame)
        {
            if (animations[current_animation].next_animation == "Loop")
            {
                SetFrame(animations[current_animation].first_frame);
            }
            else
            {
                SetAnimation(animations[current_animation].next_animation);
            }
        }
        else
        {
            SetFrame(frame + 1);
        }
    }
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

void Spritesheet::SetFlash(sf::Color color, float duration, float rate)
{
    flash_color = color;
    flash_rate = (duration / rate) / 2;;
    flash_timer = flash_rate;
    flash_complete_timer = duration;
    sprite.setColor(color);
}

sf::Sprite& Spritesheet::GetSprite()
{
    return sprite;
}

std::stringstream& operator>>(std::stringstream& ss, Spritesheet::Config& config)
{
    std::string line = ss.str();
    auto sit = line.begin();
    while (sit != line.end())
    {
        while (sit != line.end() && *sit != '[')
        {
            ++sit;
        }

        if (sit == line.end())
        {
            break;
        }

        auto s_sit = ++sit;

        while (sit != line.end() && *sit != ']')
        {
            ++sit;
        }

        std::stringstream ss(std::string(s_sit, sit));
        int left, right, width, height;
        ss >> left;
        ss >> right;
        ss >> width;
        ss >> height;
        config.frames.push_back(sf::IntRect(left, right, width, height));
    }

    return ss;
}
