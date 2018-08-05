#include "main_menu.h"

MainMenu::MainMenu() : current_menu{Menu::Main}
{
    Spritesheet::Config play_button_config;
    play_button_config.frames.push_back(sf::IntRect(0, 0, 400, 100));
    play_button_config.frames.push_back(sf::IntRect(0, 100, 400, 100));
    play_button_config.frames.push_back(sf::IntRect(0, 200, 400, 100));

    play_button = CursorButton("assets/PlayButton.png", play_button_config);
    play_button.GetSprite().setPosition(sf::Vector2f(200, 100));

    Spritesheet::Config settings_button_config;
    settings_button_config.frames.push_back(sf::IntRect(0, 0, 400, 100));
    settings_button_config.frames.push_back(sf::IntRect(0, 100, 400, 100));
    settings_button_config.frames.push_back(sf::IntRect(0, 200, 400, 100));

    settings_button = CursorButton("assets/SettingsButton.png", settings_button_config);
    settings_button.GetSprite().setPosition(sf::Vector2f(200, 250));

    Spritesheet::Config quit_button_config;
    quit_button_config.frames.push_back(sf::IntRect(0, 0, 400, 100));
    quit_button_config.frames.push_back(sf::IntRect(0, 100, 400, 100));
    quit_button_config.frames.push_back(sf::IntRect(0, 200, 400, 100));

    quit_button = CursorButton("assets/QuitButton.png", quit_button_config);
    quit_button.GetSprite().setPosition(sf::Vector2f(200, 400));
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

        }
        break;
    }
}
