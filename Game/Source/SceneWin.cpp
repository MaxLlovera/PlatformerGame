#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "FadeToBlack.h"
#include "SceneWin.h"
#include "SceneIntro.h"

#include "Defs.h"
#include "Log.h"

SceneWin::SceneWin() : Module()
{
	name.Create("SceneWin");
}

SceneWin::~SceneWin()
{

}

bool SceneWin::Awake(pugi::xml_node& node)
{
	return true;
}

bool SceneWin::Start()
{
	bool ret = true;
	if (this->active == true)
	{
		LOG("Loading background assets");
		winText = app->tex->Load("Assets/textures/sceneWin.png");
		app->player->Disable();
		app->map->Disable();
		app->audio->PlayMusic("Assets/audio/music/Victory.ogg");
		
	}
	return ret;
}

bool SceneWin::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) app->fadetoblack->FadeToBlk(this, app->sceneIntro, 1 / dt);
	//volume changes
	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) app->audio->ChangeVolume(8);
	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) app->audio->ChangeVolume(-8);
	return true;
}

// Update: draw background
bool SceneWin::PostUpdate()
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;
	app->render->DrawTexture(winText, 0, 555, NULL);
	return ret;
}

bool SceneWin::CleanUp()
{
	app->tex->UnLoad(winText);
	return true;
}