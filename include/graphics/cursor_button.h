#ifndef CURSOR_BUTTON_H
#define CURSOR_BUTTON_H

#include <functional>
#include "graphics/spritesheet.h"

class CursorButton
{
public:
    CursorButton();
    CursorButton(std::string filepath);

    void Update(sf::Time elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

    sf::Sprite& GetSprite();

    void RegisterOnClickDown(std::function<void(void)> f);
    void RegisterOnClickUp(std::function<void(void)> f);
    void RegisterOnHoverEnter(std::function<void(void)> f);
    void RegisterOnHoverExit(std::function<void(void)> f);

protected:
    void load(std::string filepath);
    virtual void onClickUp();
    virtual void onClickDown();
    virtual void onHoverEnter();
    virtual void onHoverExit();

    Spritesheet sprite;
    bool mouse_hover = false;
    bool mouse_pressed = false;

    std::vector<std::function<void(void)>> onClickDownVector;
    std::vector<std::function<void(void)>> onClickUpVector;
    std::vector<std::function<void(void)>> onHoverEnterVector;
    std::vector<std::function<void(void)>> onHoverExitVector;
};

#endif // CURSOR_BUTTON_H
