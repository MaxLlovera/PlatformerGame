#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "FadeToBlack.h"
#include "SceneWin.h"

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
	LOG("Loading background assets");
	logo = app->tex->Load("Assets/textures/sceneWin.png");
	bool ret = true;

	return ret;
}

bool SceneWin::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) app->fadetoblack->FadeToBlk(this, app->scene, 60);
	return true;
}

// Update: draw background
bool SceneWin::PostUpdate()
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;
	app->render->DrawTexture(logo, 0, 555, NULL);
	return ret;
}

bool SceneWin::CleanUp()
{
	return true;
}