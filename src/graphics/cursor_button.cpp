#include "graphics/cursor_button.h"
#include <SFML/Window/Mouse.hpp>

#include <iostream>
#include "data_file.h"

CursorButton::CursorButton()
{
}

CursorButton::CursorButton(std::string filepath)
{
    load(filepath);
    sprite.SetAnimation("Up");
}

void CursorButton::load(std::string filepath)
{
    std::string sprite_path("assets/");
    Spritesheet::Config config;
    std::vector<Spritesheet::Animation> animations;

    DataFile data_file;
    if (!data_file.Open(filepath))
    {
        std::cerr << "Exodus: Button could not be loaded: " << filepath << std::endl;
        return;
    }

    while (data_file.MoreKeys())
    {
        auto data = data_file.GetKey();
        if (data.key == "Sprite")
        {
            auto ss = data.ss;
            std::string path;
            *ss >> path;
            sprite_path += std::string(path);
        }
        else if (data.key == "Frames")
        {
            auto ss = data.ss;
            *ss >> config;
        }
        else if (data.key == "Animation")
        {
            auto ss = data.ss;
            int start_frame;
            int end_frame;
            float animation_speed;
            std::string animation_name;
            *ss >> animation_name;
            *ss >> start_frame;
            *ss >> end_frame;
            *ss >> animation_speed;

            animations.push_back(Spritesheet::Animation{animation_name, start_frame, end_frame, animation_speed});
        }
    }

    sprite = Spritesheet(sprite_path, config);
    for (auto animation : animations)
    {
        sprite.AddAnimation(animation);
    }
}

void CursorButton::Update(sf::Time elapsed, sf::RenderWindow& window)
{
    if (window.hasFocus())
    {
        sf::FloatRect bounds = sprite.GetSprite().getGlobalBounds();
        auto mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

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
}

void CursorButton::Draw(sf::RenderWindow& window)
{
    sprite.Draw(window);
}

sf::Sprite& CursorButton::GetSprite()
{
    return sprite.GetSprite();
}

void CursorButton::RegisterOnClickDown(std::function<void(void)> f)
{
    onClickDownVector.push_back(f);
}

void CursorButton::RegisterOnClickUp(std::function<void(void)> f)
{
    onClickUpVector.push_back(f);
}

void CursorButton::RegisterOnHoverEnter(std::function<void(void)> f)
{
    onHoverEnterVector.push_back(f);
}

void CursorButton::RegisterOnHoverExit(std::function<void(void)> f)
{
    onHoverExitVector.push_back(f);
}

void CursorButton::onClickUp()
{
    mouse_pressed = false;
    sprite.SetAnimation("Hover");

    for (auto callback : onClickUpVector)
    {
        callback();
    }
}

void CursorButton::onClickDown()
{
    mouse_pressed = true;
    sprite.SetAnimation("Down");

    for (auto callback : onClickDownVector)
    {
        callback();
    }
}

void CursorButton::onHoverEnter()
{
    mouse_hover = true;
    sprite.SetAnimation("Hover");

    for (auto callback : onHoverEnterVector)
    {
        callback();
    }
}

void CursorButton::onHoverExit()
{
    mouse_hover = false;
    sprite.SetAnimation("Up");

    for (auto callback : onHoverExitVector)
    {
        callback();
    }
}
