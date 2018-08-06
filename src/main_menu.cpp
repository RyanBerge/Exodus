#include "main_menu.h"
#include "settings.h"

MainMenu::MainMenu() : current_menu{Menu::Main}
{
    Spritesheet::Config button_config;
    button_config.frames.push_back(sf::IntRect(0, 0, 400, 100));
    button_config.frames.push_back(sf::IntRect(0, 100, 400, 100));
    button_config.frames.push_back(sf::IntRect(0, 200, 400, 100));

    play_button = CursorButton("assets/PlayButton.png", button_config);
    settings_button = CursorButton("assets/SettingsButton.png", button_config);
    quit_button = CursorButton("assets/QuitButton.png", button_config);
    back_button = CursorButton("assets/BackButton.png", button_config);

    quit_button.RegisterOnClickUp(std::bind(&MainMenu::Quit, this));
    play_button.RegisterOnClickUp(std::bind(&MainMenu::Play, this));
    settings_button.RegisterOnClickUp(std::bind(&MainMenu::OpenSettings, this));
    back_button.RegisterOnClickUp(std::bind(&MainMenu::Back, this));

    Resize(sf::Vector2u(Settings::video_resolution.x, Settings::video_resolution.y));
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
        }
        break;
    }
}

void MainMenu::Resize(sf::Vector2u ratio)
{
    float scale = 1;
    float x_scale = 1;
    float y_scale = 1;

    if (ratio.y < 500)
    {
        y_scale = static_cast<float>(ratio.y) / 500;
    }

    if (ratio.x < 500)
    {
        x_scale = static_cast<float>(ratio.x) / 500;
    }

    scale = (x_scale < y_scale) ? x_scale : y_scale;

    unsigned horizontal_offset = ((ratio.x - 400 * scale) / 2);
    unsigned vertical_offset = ((ratio.y - 400 * scale) / 2);
    if (vertical_offset > 100 * scale)
    {
        vertical_offset = 100 * scale;
    }

    play_button.GetSprite().setPosition(sf::Vector2f(horizontal_offset, vertical_offset));
    settings_button.GetSprite().setPosition(sf::Vector2f(horizontal_offset, vertical_offset + 150 * scale));
    quit_button.GetSprite().setPosition(sf::Vector2f(horizontal_offset, vertical_offset + 300 * scale));
    back_button.GetSprite().setPosition(sf::Vector2f(horizontal_offset, vertical_offset + 300 * scale));

    play_button.GetSprite().setScale(sf::Vector2f(scale, scale));
    settings_button.GetSprite().setScale(sf::Vector2f(scale, scale));
    quit_button.GetSprite().setScale(sf::Vector2f(scale, scale));
    back_button.GetSprite().setScale(sf::Vector2f(scale, scale));
}

void MainMenu::Play()
{
    playRequest();
}

void MainMenu::Quit()
{
    quitRequest();
}

void MainMenu::OpenSettings()
{
    current_menu = Menu::Settings;
}

void MainMenu::Back()
{
    current_menu = Menu::Main;
}

void MainMenu::RegisterPlayRequest(std::function<void(void)> f)
{
    playRequest = f;
}

void MainMenu::RegisterQuitRequest(std::function<void(void)> f)
{
    quitRequest = f;
}
