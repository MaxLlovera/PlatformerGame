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
	btnPlay->SetObserverSceneIntro(this);
	
	btnContinue = new GuiButton(2, { 480, 1025, 270, 50 }, "CONTINUE");
	btnContinue->SetObserverSceneIntro(this);
	
	btnSettings = new GuiButton(3, { 1000, 1200, 270, 50 }, "SETTINGS");
	btnSettings->SetObserverSceneIntro(this);
	
	btnCredits = new GuiButton(4, { 25, 1200, 240, 50 }, "CREDITS");
	btnCredits->SetObserverSceneIntro(this);

	btnExit = new GuiButton(5, { 1100, 570, 145, 50 }, "EXIT");
	btnExit->SetObserverSceneIntro(this);
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
	
	btnPlay->Update(dt);
	btnContinue->Update(dt);
	btnSettings->Update(dt);
	btnCredits->Update(dt);
	btnExit->Update(dt);

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

	//app->render->DrawRectangle({ 0, 555, 1280, 720 }, 100, 100, 80, 255);
	
	btnPlay->Draw();
	btnContinue->Draw();
	btnSettings->Draw();
	btnCredits->Draw();
	btnExit->Draw();

	return ret;
}

bool SceneIntro::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) app->fadetoblack->FadeToBlk(this, app->scene, 30);
		else if (control->id == 5) exit = true; // TODO: Exit request
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