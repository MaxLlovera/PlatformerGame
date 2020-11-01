#include "SceneIntro.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "SceneWin.h"
#include "SceneLose.h"
#include "FadeToBlack.h"

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
	introText = app->tex->Load("Assets/textures/sceneIntro.png");
	bool ret = true;
	app->sceneLose->Disable();
	app->sceneWin->Disable();
	app->audio->PlayMusic("no.ogg");
	return ret;
}

bool SceneIntro::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) app->fadetoblack->FadeToBlk(this, app->scene, 60);
	return true;
}

// Update: draw background
bool SceneIntro::PostUpdate()
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;
	app->render->DrawTexture(introText, 0, 555, NULL);
	return ret;
}

bool SceneIntro::CleanUp()
{
	app->tex->UnLoad(introText);
	return true;
}