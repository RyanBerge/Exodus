#include "graphics/spritesheet.h"

#include <iostream>
#include <sstream>

Spritesheet::Spritesheet()
{
}

Spritesheet::Spritesheet(std::string filepath, Config config) : texture_filepath{filepath}
{
    if (texture_filepath != "" && texture->loadFromFile(texture_filepath))
    {
        sprite.setTexture(*texture);
        is_valid = true;
        animations["Default"] = Spritesheet::Animation{"Default", 0, 0, 0, 0, 0};
        SetConfig(config);
        SetAnimation("Default");
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
    light_configs = std::vector<std::list<LightConfig>>(config.frames.size());

    hitboxes = std::vector<sf::FloatRect>(config.frames.size());
    for (unsigned i = 0; i < config.frames.size(); ++i)
    {
        hitboxes[i] = sf::FloatRect{0, 0, (float)config.frames[i].bounds.width, (float)config.frames[i].bounds.height};
    }

    SetFrame(0);
}

void Spritesheet::AddLightFrame(int frame, LightConfig light_config)
{
    if (frame >= static_cast<int>(config.frames.size()))
    {
        std::cerr << "Exodus: Spritesheet light frame index exceeds total animation frames." << std::endl;
        return;
    }

    light_configs[frame].push_back(light_config);
}

void Spritesheet::AddAnimation(Animation animation)
{
    animations[animation.name] = animation;
}

void Spritesheet::SetHitboxes(std::vector<sf::FloatRect> boxes)
{
    if (boxes.size() == 0)
    {
        return;
    }

    if (hitboxes.size() != boxes.size())
    {
        std::cerr << "Exodus: Hitbox vector does not match frame count." << std::endl;
        return;
    }

    for (unsigned i = 0; i < boxes.size(); ++i)
    {
        if (boxes[i] != sf::FloatRect{-1, -1, -1, -1} || boxes[i] == sf::FloatRect{0, 0, 0, 0})
        {
            hitboxes[i] = boxes[i];
        }
    }
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
        return true;
    }
    else
    {
        std::cerr << "Exodus: Requested animation not present: " << name << std::endl;
        return false;
    }
}

void Spritesheet::StopAnimation()
{
    current_animation = "";
}

void Spritesheet::AdvanceAnimation()
{
    if (current_animation != "" && current_animation != "Default")
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
        sprite.setTextureRect(config.frames[new_frame].bounds);
        sprite.setOrigin(config.frames[new_frame].origin);
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

std::list<Spritesheet::LightConfig> Spritesheet::GetLights()
{
    return light_configs[frame];
}

std::string Spritesheet::GetAnimation()
{
    return current_animation;
}

sf::FloatRect Spritesheet::GetHitbox()
{
    sf::FloatRect hitbox{hitboxes[frame]};
    hitbox.left += sprite.getGlobalBounds().left;
    hitbox.top += sprite.getGlobalBounds().top;
    return hitbox;
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
        int left, right, width, height, x{0}, y{0};
        ss >> left;
        ss >> right;
        ss >> width;
        ss >> height;
        ss >> x;
        ss >> y;
        config.frames.push_back(Spritesheet::Frame{sf::IntRect(left, right, width, height), sf::Vector2f(x, y)});
    }

    return ss;
}
