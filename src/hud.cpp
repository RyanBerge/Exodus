#include "hud.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "data_file.h"

Hud::Hud()
{
    load("data/ui/hud.txt");
}

void Hud::load(std::string filepath)
{
    std::string sprite_path("assets/");
    Spritesheet::Config config;

    DataFile data_file;
    if (!data_file.Open(filepath))
    {
        std::cerr << "Exodus: Room could not be loaded: " << filepath << std::endl;
        return;
    }

    while (data_file.MoreKeys())
    {
        auto data = data_file.GetKey();

        if (data.key == "Hearts")
        {
            auto ss = data.ss;

            std::string filename;
            int x, y, spacing;
            *ss >> filename;
            *ss >> x;
            *ss >> y;
            *ss >> spacing;

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
    std::string sprite_path("assets/");
    Spritesheet::Config config;
    std::vector<Spritesheet::Animation> animations;

    DataFile data_file;
    if (!data_file.Open(filepath))
    {
        std::cerr << "Exodus: HUD could not be loaded: " << filepath << std::endl;
        return Spritesheet();
    }

    while (data_file.MoreKeys())
    {
        auto data = data_file.GetKey();
        if (data.key == "Sprite")
        {
            auto ss = data.ss;
            std::string path;
            *ss >> path;
            sprite_path += std::string(path);
        }
        else if (data.key == "Frames")
        {
            auto ss = data.ss;
            *ss >> config;
        }
        else if (data.key == "Animation")
        {
            auto ss = data.ss;
            int start_frame;
            int end_frame;
            float animation_speed;
            std::string animation_name;
            *ss >> animation_name;
            *ss >> start_frame;
            *ss >> end_frame;
            *ss >> animation_speed;

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
