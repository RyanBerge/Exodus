#include "graphics/cursor_button.h"
#include <SFML/Window/Mouse.hpp>

CursorButton::CursorButton()
{

}

CursorButton::CursorButton(std::string filepath, Spritesheet::Config config) : spritesheet{filepath, config}
{
    spritesheet.SetFrame(0);
}

void CursorButton::Update(sf::Time elapsed, sf::RenderWindow& window)
{
    sf::FloatRect bounds = spritesheet.GetSprite().getGlobalBounds();
    auto mouse_position = sf::Mouse::getPosition(window);
    bool in_bounds = mouse_position.x >= bounds.left &&
                     mouse_position.x <= bounds.left + bounds.width &&
                     mouse_position.y >= bounds.top &&
                     mouse_position.y <= bounds.top + bounds.height;

    if (in_bounds && !mouse_hover)
    {
        onHoverEnter();
    }
    else if (!in_bounds && mouse_hover)
    {
        onHoverExit();
    }

    if (in_bounds && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !mouse_pressed)
    {
        onClickDown();
    }
    else if (in_bounds && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mouse_pressed)
    {
        onClickUp();
    }
}

void CursorButton::Draw(sf::RenderWindow& window)
{
    spritesheet.Draw(window);
}

sf::Sprite& CursorButton::GetSprite()
{
    return spritesheet.GetSprite();
}

void CursorButton::RegisterOnClickDown(std::function<void(void)> f)
{
    onClickDownVector.push_back(std::bind(f));
}

void CursorButton::RegisterOnClickUp(std::function<void(void)> f)
{
    onClickUpVector.push_back(std::bind(f));
}

void CursorButton::RegisterOnHoverEnter(std::function<void(void)> f)
{
    onHoverEnterVector.push_back(std::bind(f));
}

void CursorButton::RegisterOnHoverExit(std::function<void(void)> f)
{
    onHoverExitVector.push_back(std::bind(f));
}

void CursorButton::onClickUp()
{
    for (auto callback : onClickUpVector)
    {
        callback();
    }

    mouse_pressed = false;
    spritesheet.SetFrame(1);
}

void CursorButton::onClickDown()
{
    for (auto callback : onClickDownVector)
    {
        callback();
    }

    mouse_pressed = true;
    spritesheet.SetFrame(2);
}

void CursorButton::onHoverEnter()
{
    for (auto callback : onHoverEnterVector)
    {
        callback();
    }

    mouse_hover = true;
    spritesheet.SetFrame(1);
}

void CursorButton::onHoverExit()
{
    for (auto callback : onHoverExitVector)
    {
        callback();
    }

    mouse_hover = false;
    spritesheet.SetFrame(0);
}
