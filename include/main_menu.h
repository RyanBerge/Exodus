#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "graphics/checkbox_button.h"

class MainMenu
{
public:
    enum class Menu
    {
        Main, Settings
    };

    MainMenu();

    void Update(sf::Time& elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

    void Resize(sf::Vector2u ratio, sf::RenderWindow& window);

    void Play();
    void Quit();
    void OpenSettings();
    void Fullscreen();
    void Back();

private:
    void load(std::string filepath);
    Menu current_menu;

    CursorButton play_button;
    CursorButton settings_button;
    CursorButton quit_button;

    CheckboxButton fullscreen_button;
    CursorButton back_button;

    bool is_fullscreen{false};

};

#endif // MAIN_MENU_H
