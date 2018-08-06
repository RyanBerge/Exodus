#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "graphics/cursor_button.h"

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

    void Resize(sf::Vector2u ratio);

    void Play();
    void Quit();
    void OpenSettings();
    void Back();

    void RegisterPlayRequest(std::function<void(void)> f);
    void RegisterQuitRequest(std::function<void(void)> f);

private:
    Menu current_menu;

    CursorButton play_button;
    CursorButton settings_button;
    CursorButton quit_button;

    CursorButton back_button;

    std::function<void(void)> playRequest;
    std::function<void(void)> quitRequest;

};

#endif // MAIN_MENU_H
