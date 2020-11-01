#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene(){}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	if (this->active == true)
	{
		app->player->Enable();
		app->map->Enable();
		background = app->tex->Load("Assets/textures/background.png");

		// L03: DONE: Load map
		app->map->Load("world1Meta.tmx");

		// Load music
		app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	}

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
    // L02: DONE 3: Request Load / Save when pressing L/S
	if(app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();

	
	//restart from first level
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->fadetoblack->FadeToBlk(this, (Module*)app->scene, 60);
		app->render->RestartValues();
	}
	
	//restart the current level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		app->fadetoblack->FadeToBlk(this, (Module*)app->scene, 60);
		app->render->RestartValues();
	}

	//view colliders
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) app->map->colliders = !app->map->colliders;


	//god mode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) app->player->godModeEnabled = !app->player->godModeEnabled;
	
	//SceneWin
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN || app->player->win)
	{
		app->fadetoblack->FadeToBlk(this, (Module*)app->sceneWin, 60);
		app->render->RestartValues();
	}
	
	//SceneLose
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN || app->player->dead)
	{
		app->fadetoblack->FadeToBlk(this, (Module*)app->sceneLose, 60);
		app->render->RestartValues();
	}

	//volume changes
	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) app->audio->ChangeVolume(8);

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) app->audio->ChangeVolume(-8);

	//camera x
	if (app->render->counter==0||app->player->godModeEnabled)
	{
		if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && app->player->position.x > 350 && app->player->position.x <= 2400&&!app->player->ThereIsLeftWall()) app->render->camera.x += 3.0f;	
		else if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && app->player->position.x >= 350 && app->player->position.x < 2400 && !app->player->ThereIsRightWall()) app->render->camera.x -= 3.0f;
	}

	//camera y
	if (app->player->position.y < 550&&!cameraUP)
	{
		app->render->camera.y += 6.0f;
		if (app->render->camera.y >= -100) 
		{
			app->render->camera.y += 0.0f;
			cameraUP = true;
		}
	}	
	if (app->player->position.y > 700 && cameraUP)
	{
		app->render->camera.y -= 6.0f;
		if (app->render->camera.y < -550) cameraUP = false;
	}


	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;

	// Draw map
	app->render->DrawTexture(background, 0, 0);
	app->map->Draw();

	if (!app->map->keyTaken) app->map->DrawKey();
	else if (app->map->keyTaken) app->map->DrawKeyTaken();
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(background);
	return true;
}
