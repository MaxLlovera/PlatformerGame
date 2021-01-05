#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "GuiButton.h"
#include "Scene.h"
#include "FadeToBlack.h"
#include "SceneLose.h"
#include "SceneIntro.h"

#include "Defs.h"
#include "Log.h"

SceneLose::SceneLose() : Module()
{
	name.Create("SceneWin");
}

SceneLose::~SceneLose()
{

}

bool SceneLose::Awake(pugi::xml_node& node)
{
	return true;
}

bool SceneLose::Start()
{
	bool ret = true;
	
	LOG("Loading background assets");
	loseText = app->tex->Load("Assets/Textures/scene_lose.png");
	//app->player->Disable();
	//app->enemy->Disable();
	//app->scene->player->dead = false;
	//app->scene->player->active = false;
	//app->scene->enemy->active = false;
	//app->scene->flyingEnemy->active = false;
	//app->flyingEnemy->Disable();
	app->map->Disable();
	app->audio->PlayMusic("Assets/Audio/Music/game_over.ogg");
	app->map->checkpointTaken = false;
	app->render->camera.x = 0;
	app->render->camera.y = -556;
	btnPlay = new GuiButton(1, { 490, 1030, 290, 50 }, " RESTART");
	btnPlay->SetObserver(this);
		
	
	

	return ret;
}

bool SceneLose::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) app->fadetoblack->FadeToBlk(this, app->sceneIntro, 1 / dt);
	//volume changes
	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) app->audio->ChangeVolume(8);
	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) app->audio->ChangeVolume(-8);
	
	btnPlay->Update(dt);

	return true;
}

// Update: draw background
bool SceneLose::PostUpdate()
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;
	app->render->DrawTexture(loseText, 0, 555, NULL);

	btnPlay->Draw();
	return ret;
}

bool SceneLose::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) app->fadetoblack->FadeToBlk(this, app->sceneIntro, 30);
	}
	default: break;
	}

	return true;
}

bool SceneLose::CleanUp()
{
	app->tex->UnLoad(loseText);
	return true;
}