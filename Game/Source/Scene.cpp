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
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{

	background = app->tex->Load("Assets/textures/background.png");

	// L03: DONE: Load map
	app->map->Load("world1Meta.tmx");
	//app->map->Load("iso.tmx");

	// Load music
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

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
		app->fadetoblack->FadeToBlk((Module*)app->scene, (Module*)app->scene, 60.0f);
		app->render->RestartValues();
	}
	
	//restart the current level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) app->render->RestartValues();

	//view colliders
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) app->map->colliders = !app->map->colliders;


	//god mode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KeyState::KEY_DOWN) app->player->godModeEnabled = !app->player->godModeEnabled;
	
	if (app->input->GetKey(SDL_SCANCODE_F11) == KeyState::KEY_DOWN) app->fadetoblack->FadeToBlk((Module*)app->scene, (Module*)app->scene, 60);

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
	
	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->data.width, app->map->data.height,
				   app->map->data.tileWidth, app->map->data.tileHeight,
				   app->map->data.tilesets.count());

	app->win->SetTitle(title.GetString());

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

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
