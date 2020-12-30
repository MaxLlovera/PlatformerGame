#include "SceneIntro.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "SceneManager.h"

#include "EntityManager.h"

#include "Defs.h"
#include "Log.h"

SceneIntro::SceneIntro()
{
	name.Create("sceneIntro");
}

SceneIntro::~SceneIntro()
{

}

bool SceneIntro::Awake()
{
	return true;
}

bool SceneIntro::Start()
{
	LOG("Loading background assets");
	introText = app->tex->Load("Assets/Textures/scene_intro.png");
	bool ret = true;
	//app->sceneLose->Disable();
	//app->sceneWin->Disable();
	app->audio->PlayMusic("Assets/Audio/Music/intro_theme.ogg");
	app->render->camera.x = 0;
	app->render->camera.y = -555;
	//app->player->position.x = 350;
	//app->player->position.y = 875;

	//if (app->player->win) app->SaveGameRequest();

	//app->player->win = false;
	return ret;
}

bool SceneIntro::Update(float dt)
{

	

	//volume changes
	//if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) app->audio->ChangeVolume(8);
	//if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) app->audio->ChangeVolume(-8);
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