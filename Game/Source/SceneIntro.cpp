#include "SceneIntro.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "SceneWin.h"
#include "SceneLose.h"
#include "FadeToBlack.h"
#include "GuiButton.h"
#include "Font.h"

#include "Defs.h"
#include "Log.h"

SceneIntro::SceneIntro() : Module()
{
	name.Create("sceneIntro");

	

}

SceneIntro::~SceneIntro()
{

}

bool SceneIntro::Awake(pugi::xml_node& node)
{
	return true;
}

bool SceneIntro::Start()
{
	LOG("Loading background assets");
	introText = app->tex->Load("Assets/Textures/scene_intro.png");
	bool ret = true;
	app->sceneLose->Disable();
	app->sceneWin->Disable();
	app->audio->PlayMusic("Assets/Audio/Music/intro_theme.ogg");
	app->render->camera.x = 0;
	app->render->camera.y = -555;
	//app->scene->player->position.x = 350;
	//app->scene->player->position.y = 875;
	//if (app->scene->player->win) app->SaveGameRequest();
	//app->scene->player->win = false;
	char lookupTable[] = { "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ:/,!+-%  " };
	whiteFont = app->font->Load("Assets/Textures/white_font.png", lookupTable, 9);

	btnPlay = new GuiButton(1, { 540, 950, 145, 50 }, "PLAY");
	btnPlay->SetObserver(this);
	
	btnContinue = new GuiButton(2, { 480, 1025, 270, 50 }, "CONTINUE");
	btnContinue->SetObserver(this);
	
	btnSettings = new GuiButton(3, { 1000, 1200, 270, 50 }, "SETTINGS");
	btnSettings->SetObserver(this);
	
	btnCredits = new GuiButton(4, { 25, 1200, 240, 50 }, "CREDITS");
	btnCredits->SetObserver(this);

	btnExit = new GuiButton(5, { 1100, 570, 145, 50 }, "EXIT");
	btnExit->SetObserver(this);
	
	btnBackSettings = new GuiButton(6, { 540, 1200, 145, 50 }, "BACK");
	btnBackSettings->SetObserver(this);
	
	btnBackCredits = new GuiButton(7, { 540, 1200, 145, 50 }, "BACK");
	btnBackCredits->SetObserver(this);
	return ret;
}

bool SceneIntro::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->render->RestartValues();
		//app->fadetoblack->FadeToBlk(this, (Scene*)app->sceneManager->scene1, 1 / dt);
		app->fadetoblack->FadeToBlk(this, app->scene, 1 / dt);
		//app->fadetoblack->FadeToBlk(this, app->scene, 1 / dt);
	}
	if (settings == true)
	{
		btnExit->Update(dt);
		btnBackSettings->Update(dt);
		btnSettings->Update(dt);
		btnCredits->Update(dt);
	}
	else if (credits == true)
	{
		btnExit->Update(dt);
		btnBackCredits->Update(dt);
		btnSettings->Update(dt);
		btnCredits->Update(dt);
	}
	else
	{
		btnPlay->Update(dt);
		btnContinue->Update(dt);
		btnSettings->Update(dt);
		btnCredits->Update(dt);
		btnExit->Update(dt);
	}


	//volume changes
	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) app->audio->ChangeVolume(8);
	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) app->audio->ChangeVolume(-8);
	if (exit == true) return false;
	return true;
}

// Update: draw background
bool SceneIntro::PostUpdate()
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;
	app->render->DrawTexture(introText, 0, 555, NULL);

	if (settings == true)
	{
		app->render->DrawRectangle({ 150, 900, 950, 280 }, 150, 50, 100, 150);
		btnBackSettings->Draw();
		btnExit->Draw();
		btnCredits->Draw();
		btnSettings->Draw();
	}
	else if (credits == true)
	{
		btnBackCredits->Draw();
		btnExit->Draw();
		btnCredits->Draw();
		btnSettings->Draw();
	}
	else
	{
		btnPlay->Draw();
		btnContinue->Draw();
		btnSettings->Draw();
		btnCredits->Draw();
		btnExit->Draw();
	}
	

	return ret;
}

bool SceneIntro::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) app->fadetoblack->FadeToBlk(this, app->scene, 30);
		else if (control->id == 3)
		{
			btnCredits->state = GuiControlState::DISABLED;
			btnSettings->state = GuiControlState::DISABLED;
			settings = true;
		}
		else if (control->id == 4)
		{
			btnCredits->state = GuiControlState::DISABLED;
			btnSettings->state = GuiControlState::DISABLED;
			credits = true;
		}
		else if (control->id == 5) exit = true;
		else if (control->id == 6)
		{
			btnCredits->state = GuiControlState::NORMAL;
			btnSettings->state = GuiControlState::NORMAL;
			settings = false;
		}
		else if (control->id == 7)
		{
			btnCredits->state = GuiControlState::NORMAL;
			btnSettings->state = GuiControlState::NORMAL;
			credits = false;
		}
	}
	default: break;
	}

	return true;
}

bool SceneIntro::CleanUp()
{
	app->tex->UnLoad(introText);
	return true;
}