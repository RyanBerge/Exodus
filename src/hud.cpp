#include "hud.h"

#include <fstream>
#include <iostream>
#include <sstream>

Hud::Hud()
{
    load("data/ui/hud.txt");
}

void Hud::load(std::string filepath)
{
    std::ifstream file(filepath);
    std::string line;

    std::string sprite_path("assets/");
    Spritesheet::Config config;

    if (!file.is_open())
    {
        std::cerr << "Exodus: Hud was not loaded: " << filepath << std::endl;
        return;
    }

    while (!file.eof())
    {
        std::getline(file, line);
        if (line == "")
        {
            continue;
        }

        auto sit = line.begin();
        std::string key;
        while (sit != line.end() && *sit != ':')
        {
            key += *sit;
            ++sit;
        }

        while (sit != line.end() && (*sit == ' ' || *sit == ':'))
        {
            ++sit;
        }

        if (key == "Hearts")
        {
            std::stringstream ss(std::string(sit, line.end()));
            std::string filename;
            int x, y, spacing;
            ss >> filename;
            ss >> x;
            ss >> y;
            ss >> spacing;

            Spritesheet heart = load_heart("data/ui/" + filename + ".txt");

            for (int i = 0; i < max_health; ++i)
            {
                Spritesheet sprite = heart;
                sprite.GetSprite().setPosition(x + i * spacing + HUD_DRAW_X, y + HUD_DRAW_Y);
                hearts.push_back(sprite);
            }

            SetHealth(max_health);
        }
    }
}

Spritesheet Hud::load_heart(std::string filepath)
{
    std::ifstream file(filepath);
    std::string line;

    std::string sprite_path("assets/");
    Spritesheet::Config config;

    if (!file.is_open())
    {
        std::cerr << "Exodus: Heart was not loaded: " << filepath << std::endl;
        return Spritesheet();
    }

    std::vector<Spritesheet::Animation> animations;

    while (!file.eof())
    {
        std::getline(file, line);
        if (line == "")
        {
            continue;
        }

        auto sit = line.begin();
        std::string key;
        while (sit != line.end() && *sit != ':')
        {
            key += *sit;
            ++sit;
        }

        while (sit != line.end() && (*sit == ' ' || *sit == ':'))
        {
            ++sit;
        }

        if (key == "Sprite")
        {
            sprite_path += std::string(sit, line.end());
        }
        else if (key == "Frames")
        {
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
        }
        else if (key == "Animation")
        {
            std::stringstream ss(std::string(sit, line.end()));
            int start_frame;
            int end_frame;
            float animation_speed;
            std::string animation_name;
            ss >> animation_name;
            ss >> start_frame;
            ss >> end_frame;
            ss >> animation_speed;

            animations.push_back(Spritesheet::Animation{animation_name, start_frame, end_frame, animation_speed});
        }
    }

    Spritesheet sprite(sprite_path, config);
    for (auto animation : animations)
    {
        sprite.AddAnimation(animation);
    }

    return sprite;
}

void Hud::Update(sf::Time elapsed, sf::RenderWindow& window)
{
}

void Hud::Draw(sf::RenderWindow& window)
{
    sf::View old_view = window.getView();
    sf::View view(sf::FloatRect(HUD_DRAW_X, HUD_DRAW_Y, 1200, 800));
    view.setViewport(viewport);
    window.setView(view);
    for (auto& heart : hearts)
    {
        heart.Draw(window);
    }
    window.setView(old_view);
}

void Hud::SetHealth(int hp)
{
    health = hp;
    for (int i = 0; i < health; ++i)
    {
        hearts[i].SetAnimation("Lit");
    }

    for (int i = health; i < max_health; ++i)
    {
        hearts[i].SetAnimation("Unlit");
    }
}

void Hud::SetViewport(sf::FloatRect viewport)
{
    this->viewport = viewport;
}
