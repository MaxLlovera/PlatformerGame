#include "GuiButton.h"
#include "App.h"
#include "Audio.h"
#include "Font.h"
#include "Window.h"
#include "Input.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	//bounds.x /= app->win->GetScale();
	//bounds.y /= app->win->GetScale();
	this->bounds = bounds;
	this->text = text;

	guiButtonFx = app->audio->LoadFx("Assets/Audio/Fx/gui_button_fx.wav");
	guiButtonErrorFx = app->audio->LoadFx("Assets/Audio/Fx/gui_button_error.wav");
	guiButtonMoveFx = app->audio->LoadFx("Assets/Audio/Fx/gui_button_move.wav");
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(float dt)
{
	
	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		mouseX += -app->render->camera.x / app->win->GetScale();
		mouseY += -app->render->camera.y / app->win->GetScale();

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;
			if (!soundDone)
			{
				app->audio->PlayFx(guiButtonMoveFx, 0);
				soundDone = true;
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				
				state = GuiControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				app->audio->PlayFx(guiButtonFx, 0);
				NotifyObserver();
			}
		}
		
		else
		{
			state = GuiControlState::NORMAL;
			soundDone = false;
		}
		
	}
	else if (state == GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		mouseX += -(app->render->camera.x) / (int)app->win->GetScale();
		mouseY += -(app->render->camera.y) / (int)app->win->GetScale();

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				app->audio->PlayFx(guiButtonErrorFx, 0);
			}
		}
	}

	return false;
}

bool GuiButton::Draw()
{
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED: app->render->DrawRectangle(bounds, 100, 100, 100, 255);
		break;
	case GuiControlState::NORMAL: app->render->DrawRectangle(bounds, 140, 75, 185, 255);
		break;
	case GuiControlState::FOCUSED: app->render->DrawRectangle(bounds, 200, 100, 220, 255);
		break;
	case GuiControlState::PRESSED: app->render->DrawRectangle(bounds, 250, 75, 150, 255);
		break;
	case GuiControlState::SELECTED: app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;
	default:
		break;
	}
	app->font->DrawText(bounds.x + (app->render->camera.x) + 8, bounds.y + (app->render->camera.y) + 8, 0, text.GetString());
	return false;
}
