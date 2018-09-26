#include "main_menu.h"
#include "settings.h"

#include "data_file.h"
#include "global.h"
#include <iostream>

MainMenu::MainMenu() : current_menu{Menu::Main}
{
    load("data/ui/menu/main_menu.txt");

    quit_button.RegisterOnClickUp(std::bind(&MainMenu::Quit, this));
    play_button.RegisterOnClickUp(std::bind(&MainMenu::Play, this));
    settings_button.RegisterOnClickUp(std::bind(&MainMenu::OpenSettings, this));
    fullscreen_button.RegisterOnClickUp(std::bind(&MainMenu::Fullscreen, this));
    back_button.RegisterOnClickUp(std::bind(&MainMenu::Back, this));
}

void MainMenu::load(std::string filepath)
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

            if (path == "play")
            {
                play_button = CursorButton("data/ui/menu/" + path + ".txt");
                play_button.GetSprite().setPosition(x, y);
            }
            else if (path == "settings")
            {
                settings_button = CursorButton("data/ui/menu/" + path + ".txt");
                settings_button.GetSprite().setPosition(x, y);
            }
            else if (path == "quit")
            {
                quit_button = CursorButton("data/ui/menu/" + path + ".txt");
                quit_button.GetSprite().setPosition(x, y);
            }
            else if (path == "back")
            {
                back_button = CursorButton("data/ui/menu/" + path + ".txt");
                back_button.GetSprite().setPosition(x, y);
            }
        }
        else if (data.key == "CheckboxButton")
        {
            auto ss = data.ss;
            std::string path;
            float x, y;
            *ss >> path;
            *ss >> x;
            *ss >> y;

            if (path == "fullscreen")
            {
                fullscreen_button = CheckboxButton("data/ui/menu/" + path + ".txt");
                fullscreen_button.GetSprite().setPosition(x, y);
            }
        }
    }
}

void MainMenu::Update(sf::Time& elapsed, sf::RenderWindow& window)
{
    switch (current_menu)
    {
        case Menu::Main:
        {
            play_button.Update(elapsed, window);
            settings_button.Update(elapsed, window);
            quit_button.Update(elapsed, window);
        }
        break;
        case Menu::Settings:
        {
            back_button.Update(elapsed, window);
            fullscreen_button.Update(elapsed, window);
        }
        break;
    }
}

void MainMenu::Draw(sf::RenderWindow& window)
{
    switch (current_menu)
    {
        case Menu::Main:
        {
            play_button.Draw(window);
            settings_button.Draw(window);
            quit_button.Draw(window);
        }
        break;
        case Menu::Settings:
        {
            back_button.Draw(window);
            fullscreen_button.Draw(window);
        }
        break;
    }
}

void MainMenu::Resize(sf::Vector2u ratio, sf::RenderWindow& window)
{
    float aspect_ratio = 1200.f / 800.f;
    float window_ratio = static_cast<float>(ratio.x) / (static_cast<float>(ratio.y));

    float viewport_width = 1;
    float viewport_height = 1;
    float viewport_x = 0;
    float viewport_y = 0;

    if (window_ratio > aspect_ratio)
    {
        viewport_width = aspect_ratio / window_ratio;
        viewport_x = (1 - viewport_width) / 2;
    }
    else if (window_ratio < aspect_ratio)
    {
        viewport_height = window_ratio / aspect_ratio;
        viewport_y = (1 - viewport_height) / 2;
    }

    sf::FloatRect view_rect{0, 0, 1200, 800};
    view_rect.left = window.getView().getCenter().x - window.getView().getSize().x / 2;
    view_rect.top = window.getView().getCenter().y - window.getView().getSize().y / 2;

    sf::View view(view_rect);
    view.setViewport(sf::FloatRect(viewport_x, viewport_y, viewport_width, viewport_height));
    window.setView(view);
}

void MainMenu::Play()
{
    Global::Play();
}

void MainMenu::Quit()
{
    Global::Quit();
}

void MainMenu::OpenSettings()
{
    current_menu = Menu::Settings;
}

void MainMenu::Fullscreen()
{
    is_fullscreen = !is_fullscreen;
    Global::Fullscreen(is_fullscreen);
}

void MainMenu::Back()
{
    current_menu = Menu::Main;
}

