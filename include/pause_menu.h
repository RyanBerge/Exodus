#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "graphics/cursor_button.h"

class PauseMenu
{
public:
    PauseMenu();

    void Initialize();

    void Update(sf::Time& elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

    void Resume();
    void QuitToMenu();

private:
    void load(std::string filepath);

    Spritesheet background;
    CursorButton resume_button;
    CursorButton quit_button;
};

#endif // PAUSE_MENU_H
