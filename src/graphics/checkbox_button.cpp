#include "graphics/checkbox_button.h"

CheckboxButton::CheckboxButton(std::string filepath)
{
    load(filepath);
    sprite.SetAnimation("Up");
}

bool CheckboxButton::IsChecked()
{
    return checked;
}

void CheckboxButton::onClickUp()
{
    mouse_pressed = false;
    if (!checked)
    {
        sprite.SetAnimation("Hover");
        checked = true;
    }
    else
    {
        sprite.SetAnimation("HoverChecked");
        checked = false;
    }

    for (auto callback : onClickUpVector)
    {
        callback();
    }
}

void CheckboxButton::onClickDown()
{
    mouse_pressed = true;
    if (!checked)
    {
        sprite.SetAnimation("Down");
    }
    else
    {
        sprite.SetAnimation("DownChecked");
    }

    for (auto callback : onClickDownVector)
    {
        callback();
    }
}

void CheckboxButton::onHoverEnter()
{
    mouse_hover = true;
    if (!checked)
    {
        sprite.SetAnimation("Hover");
    }
    else
    {
        sprite.SetAnimation("HoverChecked");
    }

    for (auto callback : onHoverEnterVector)
    {
        callback();
    }
}

void CheckboxButton::onHoverExit()
{
    mouse_hover = false;
    if (!checked)
    {
        sprite.SetAnimation("Up");
    }
    else
    {
        sprite.SetAnimation("UpChecked");
    }

    for (auto callback : onHoverExitVector)
    {
        callback();
    }
}
