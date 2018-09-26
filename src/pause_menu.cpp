#include "pause_menu.h"

#include "data_file.h"
#include "global.h"
#include <iostream>

PauseMenu::PauseMenu()
{
    load("data/ui/pause_menu/pause_menu.txt");
}

void PauseMenu::Initialize()
{
    resume_button.RegisterOnClickUp(Global::Resume);
    quit_button.RegisterOnClickUp(Global::QuitToMenu);
}

void PauseMenu::load(std::string filepath)
{
    DataFile data_file;
    if (!data_file.Open(filepath))
    {
        std::cerr << "Exodus: Menu could not be loaded: " << filepath << std::endl;
        return;
    }

    while (data_file.MoreKeys())
    {
        auto data = data_file.GetKey();
        if (data.key == "Button")
        {
            auto ss = data.ss;
            std::string path;
            float x, y;
            *ss >> path;
            *ss >> x;
            *ss >> y;

            if (path == "resume")
            {
                resume_button = CursorButton("data/ui/pause_menu/" + path + ".txt");
                resume_button.GetSprite().setPosition(x, y);
            }
            else if (path == "quit_to_menu")
            {
                quit_button = CursorButton("data/ui/pause_menu/" + path + ".txt");
                quit_button.GetSprite().setPosition(x, y);
            }
        }
        else if (data.key == "Background")
        {
            auto ss = data.ss;

            std::string sprite_path;
            int x, y;

            *ss >> sprite_path;
            *ss >> x;
            *ss >> y;

            Spritesheet::Config config;
            config.frames.push_back(Spritesheet::Frame{sf::IntRect(0, 0, 800, 700), sf::Vector2f(0, 0)});
            background = Spritesheet("assets/" + sprite_path, config);
            background.SetFrame(0);
            background.GetSprite().setPosition(x, y);
        }
    }
}

void PauseMenu::Update(sf::Time& elapsed, sf::RenderWindow& window)
{
    background.Update(elapsed, window);
    resume_button.Update(elapsed, window);
    quit_button.Update(elapsed, window);
}

void PauseMenu::Draw(sf::RenderWindow& window)
{
    background.Draw(window);
    resume_button.Draw(window);
    quit_button.Draw(window);
}
