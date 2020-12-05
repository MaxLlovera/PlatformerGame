#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "FadeToBlack.h"
#include "PathFinding.h"

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
	// L12b: Create walkability map on map loading
	if (this->active == true)
	{
		app->player->Enable();
		app->enemy->Enable();
		app->flyingEnemy->Enable();
		app->map->Enable();
		background = app->tex->Load("Assets/Textures/background.png");
		heart = app->tex->Load("Assets/Textures/head_shock.png");
		// L03: DONE: Load map
		if (app->map->Load("world_meta.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}


		// Load music
		app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
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
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();
	}
	if(app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();

	
	//restart from first level
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->fadetoblack->FadeToBlk(this, (Module*)app->scene, 1 / dt);
		app->render->RestartValues();
	}

	//restart when dies
	if (app->player->spiked && !app->player->dead)
	{

		app->fadetoblack->FadeToBlk(this, (Module*)app->scene, 1 / dt);
		app->render->RestartValues();

	}
	
	//restart the current level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		app->fadetoblack->FadeToBlk(this, (Module*)app->scene, 1 / dt);
		app->render->RestartValues();
	}

	//view colliders
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) app->map->colliders = !app->map->colliders;


	//god mode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) app->player->godModeEnabled = !app->player->godModeEnabled;
	
	//SceneWin
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN || app->player->win)
	{
		app->fadetoblack->FadeToBlk(this, (Module*)app->sceneWin, 1 / dt);
		app->render->RestartValues();
	}
	
	//SceneLose
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN || app->player->dead)
	{
		app->fadetoblack->FadeToBlk(this, (Module*)app->sceneLose, 1 / dt);
		app->render->RestartValues();
	}


	//volume changes
	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) app->audio->ChangeVolume(8);

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) app->audio->ChangeVolume(-8);

	//camera x
	if (app->render->counter==0||app->player->godModeEnabled)
	{
		if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && app->player->position.x > 350 && app->player->position.x <= 4400 && !app->player->ThereIsLeftWall() && !app->player->ThereIsChestLeft()) app->render->camera.x += 3.0f;
		else if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && app->player->position.x >= 350 && app->player->position.x < 4400 && !app->player->ThereIsRightWall() && !app->player->ThereIsChestRight()) app->render->camera.x -= 3.0f;
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
	if (app->player->position.y > 551 && cameraUP)
	{
		app->render->camera.y -= 6.0f;
		if (app->render->camera.y < -550) cameraUP = false;
	}

	//camera movement (comented)
	// L08: DONE 6: Make the camera movement independent of framerate
	/*if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += floor(200.0f * dt);

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= floor(200.0f * dt);

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += floor(200.0f * dt);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= floor(200.0f * dt);*/
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
	app->map->DrawColliders();
	app->map->DrawKey();
	app->map->DrawCheckpoint();
	app->map->DrawPuzzle();
	if (!app->map->chestTaken) app->map->DrawChest();
	if (app->map->chestTaken) app->map->DrawHeart();
	if (app->player->lifes == 4)
	{
		app->render->DrawTexture(heart, -app->render->camera.x, -app->render->camera.y);
		app->render->DrawTexture(heart, -app->render->camera.x + 64, -app->render->camera.y);
		app->render->DrawTexture(heart, -app->render->camera.x + 128, -app->render->camera.y);
		app->render->DrawTexture(heart, -app->render->camera.x + 192, -app->render->camera.y);
	}
	if (app->player->lifes == 3)
	{
		app->render->DrawTexture(heart, -app->render->camera.x, -app->render->camera.y);
		app->render->DrawTexture(heart, -app->render->camera.x + 64, -app->render->camera.y);
		app->render->DrawTexture(heart, -app->render->camera.x + 128, -app->render->camera.y);
	}
	if (app->player->lifes == 2)
	{
		app->render->DrawTexture(heart, -app->render->camera.x, -app->render->camera.y);
		app->render->DrawTexture(heart, -app->render->camera.x + 64, -app->render->camera.y);
	}
	if (app->player->lifes == 1)
	{
		app->render->DrawTexture(heart, -app->render->camera.x, -app->render->camera.y);
	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(background);
	app->tex->UnLoad(heart);

	return true;
}
