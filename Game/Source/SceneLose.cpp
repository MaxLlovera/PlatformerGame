#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "FadeToBlack.h"
#include "SceneLose.h"

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
	LOG("Loading background assets");
	logo = app->tex->Load("Assets/textures/sceneLose.png");
	bool ret = true;

	return ret;
}

bool SceneLose::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) app->fadetoblack->FadeToBlk(this, app->scene, 60);
	return true;
}

// Update: draw background
bool SceneLose::PostUpdate()
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;
	app->render->DrawTexture(logo, 0, 555, NULL);
	return ret;
}

bool SceneLose::CleanUp()
{
	return true;
}