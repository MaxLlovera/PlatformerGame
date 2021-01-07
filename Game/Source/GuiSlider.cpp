#include "GuiSlider.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Font.h"
#include "Window.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;
    this->minValue = bounds.x;
    this->maxValue = bounds.x+ bounds.w;

}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        // Camera offset applied to the mouse so we can use the options.
        mouseX += -app->render->camera.x / app->win->GetScale();
        mouseY += -app->render->camera.y / app->win->GetScale();

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
                state = GuiControlState::PRESSED;
        }
        else if (state == GuiControlState::PRESSED)
        {
            if ((app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) && (mouseX > minValue + 3) && (mouseX < maxValue + 3))
            {
                bounds.x = ((mouseX)-((bounds.w / 2) / app->win->GetScale()));
                NotifyObserver();
            }
            else if ((app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) && (id == 2))
            {
                //app->audio->PlayFx(guiButtonFx, 0);
            }
            else
            {
                state = GuiControlState::NORMAL;
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiSlider::Draw()
{
    // Draw the right button depending on state

    SDL_Rect sect = { bounds.x - 1, bounds.y - 1, 300, 30 };
    app->render->DrawRectangle(sect, 0, 255, 100, 255);


    switch (state)
    {
    case GuiControlState::DISABLED: app->render->DrawRectangle(bounds, 100, 100, 100, 255);
        break;
    case GuiControlState::NORMAL: app->render->DrawRectangle(bounds, 140, 75, 185, 255);
        break;
    case GuiControlState::FOCUSED: app->render->DrawRectangle(bounds, 200, 100, 220, 255);
        break;
    case GuiControlState::PRESSED: app->render->DrawRectangle(bounds, 0, 255, 255, 255);
        break;
    case GuiControlState::SELECTED: app->render->DrawRectangle(bounds, 0, 255, 0, 255);
        break;
    default:
        break;
    }
    app->font->DrawText(bounds.x + (app->render->camera.x) - 400 / app->win->GetScale(), bounds.y + (app->render->camera.y) - 2 / app->win->GetScale(), 0, text.GetString());
    return false;
}
