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

private:
    Menu current_menu;

    CursorButton play_button;
    CursorButton settings_button;
    CursorButton quit_button;

};

#endif // MAIN_MENU_H
