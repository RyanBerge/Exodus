#ifndef CHECKBOX_BUTTON_H
#define CHECKBOX_BUTTON_H

#include "cursor_button.h"

class CheckboxButton : public CursorButton
{
public:
    CheckboxButton() = default;
    CheckboxButton(std::string filepath);
    bool IsChecked();

private:
    virtual void onClickUp() override;
    virtual void onClickDown() override;
    virtual void onHoverEnter() override;
    virtual void onHoverExit() override;

    bool checked{false};
};

#endif // CHECKBOX_BUTTON_H
