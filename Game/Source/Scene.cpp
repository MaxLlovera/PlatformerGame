#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

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
	if(app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	//camera x
	if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)&&app->player->position.x>350&& app->player->position.x<=2400)
	{
		app->render->camera.x += 3.0f;
	}
	else if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && app->player->position.x >= 350 && app->player->position.x < 2400)
	{
		app->render->camera.x -= 3.0f;
	}
	
	

	//camera y
	if (app->player->position.y < 500&&!cameraUP)
	{
		app->render->camera.y += 3.0f;
		if (app->player->position.y < 300) 
		{
			app->render->camera.y += 0.0f;
			cameraUP = true;
		}
	}
	
	if (app->player->position.y > 700 && cameraUP)
	{
		if (app->render->camera.y != app->player->position.y - (app->render->camera.h * 2) + 10)
		{
			app->render->camera.y -= 3.0f;
			if (app->render->camera.y == app->player->position.y - (app->render->camera.h * 2) + 10)
			{
				cameraUP = false;
			}
		}
		
		/*if(app->player->position.y >= 875)
			cameraUP = false;*/
		
	}
	//else if (app->player->position.y > 850 && cameraDOWN)
	//{
	//	app->render->camera.y -= 0.0f;
	//	//cameraUP = false;	
	//	cameraDOWN = false;
	//}

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	// Draw map
	app->render->DrawTexture(background, 0, 0);
	app->map->Draw();
	
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

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
