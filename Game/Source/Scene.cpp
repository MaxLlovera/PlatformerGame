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
#include "EntityManager.h"
#include "FadeToBlack.h"
#include "PathFinding.h"
#include "SceneIntro.h"
#include "SceneLose.h"
#include "SceneWin.h"
#include "ModuleParticles.h"
#include "GuiButton.h"
#include "GuiSlider.h"


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
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		flyingEnemy = (FlyingEnemy*)app->entityManager->CreateEntity(EntityType::FLYING_ENEMY);
		particles = (ModuleParticles*)app->entityManager->CreateEntity(EntityType::PARTICLE);

		player->Start();
		//player->active = true;
		enemy->Start();
		flyingEnemy->Start();
		particles->Start();

		app->map->Enable();
		background = app->tex->Load("Assets/Textures/background.png");
		heart = app->tex->Load("Assets/Textures/head_life.png");
		key = app->tex->Load("Assets/Textures/key.png");
		puzzle = app->tex->Load("Assets/Textures/puzzle.png");
		pause = app->tex->Load("Assets/Textures/pause.png");
		player->spiked = false;
		app->map->checkpointTaken = false;





		// L03: DONE: Load map
		if (app->map->Load("world_meta.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}

		/*if (app->sceneIntro->posContinue && !app->sceneWin->wined && !app->sceneLose->losed) {
			app->LoadGameRequest();
		}*/

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
	cameraX = app->render->camera.x;
	cameraY = app->render->camera.y;

	// L02: DONE 3: Request Load / Save when pressing L/S
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();


	//restart from first level
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
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
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) player->godModeEnabled = !player->godModeEnabled;

	//cap fps
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) app->capped = !app->capped;

	//SceneWin
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN || player->win)
	{
		app->sceneWin->won = true;
		player->win = true;
		app->fadetoblack->FadeToBlk(this, (Module*)app->sceneWin, 1 / dt);

		app->render->RestartValues();
	}

	//SceneLose
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		app->sceneLose->lost = true;
		app->fadetoblack->FadeToBlk(this, (Module*)app->sceneLose, 1 / dt);
		app->render->RestartValues();
	}

	//camera x
	if ((app->render->counter == 0 || player->godModeEnabled) && !player->spiked&&!paused)
	{
		if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && player->position.x > 350 && player->position.x <= 4400 && !player->ThereIsLeftWall() && !player->ThereIsChestLeft()) app->render->camera.x += 3.0f;
		else if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && player->position.x >= 350 && player->position.x < 4400 && !player->ThereIsRightWall() && !player->ThereIsChestRight() && !player->ThereIsLeftWall()) app->render->camera.x -= 3.0f;
	}
	//camera y

	if (player->position.y < 570 && app->render->camera.y < -100) app->render->camera.y += 4.0f;

	if (player->position.y >= 570 && app->render->camera.y > -550) app->render->camera.y -= 4.0f;


	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		paused = true;
		Pause();
		
	}
	if (paused)
	{
		btnResume->Update(dt);
		btnSettings->Update(dt);
		btnBackIntro->Update(dt);
		btnExit->Update(dt);
	}
	else if (pausedSettings)
	{

	}
	if (app->sceneIntro->exit == true) return false;
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;


	// Draw map
	app->render->DrawTexture(background, 0, 0);
	app->map->Draw();
	app->map->DrawColliders();
	app->map->DrawKey();
	app->map->DrawCheckpoint();
	app->map->DrawPuzzle();
	if (!app->map->chestTaken) app->map->DrawChest();
	if (app->map->chestTaken) app->map->DrawHeart();
	if (player->lifes == 4)
	{
		app->render->DrawTexture(heart, -app->render->camera.x, -app->render->camera.y);
		app->render->DrawTexture(heart, -app->render->camera.x + 64, -app->render->camera.y);
		app->render->DrawTexture(heart, -app->render->camera.x + 128, -app->render->camera.y);
		app->render->DrawTexture(heart, -app->render->camera.x + 192, -app->render->camera.y);
	}
	if (player->lifes == 3)
	{
		app->render->DrawTexture(heart, -app->render->camera.x, -app->render->camera.y);
		app->render->DrawTexture(heart, -app->render->camera.x + 64, -app->render->camera.y);
		app->render->DrawTexture(heart, -app->render->camera.x + 128, -app->render->camera.y);
	}
	if (player->lifes == 2)
	{
		app->render->DrawTexture(heart, -app->render->camera.x, -app->render->camera.y);
		app->render->DrawTexture(heart, -app->render->camera.x + 64, -app->render->camera.y);
	}
	if (player->lifes == 1)
	{
		app->render->DrawTexture(heart, -app->render->camera.x, -app->render->camera.y);
	}
	if(app->map->keyTaken) app->render->DrawTexture(key, -app->render->camera.x + 10, -app->render->camera.y + 75);

	if(app->map->puzzleTaken&&!app->map->chestTaken) app->render->DrawTexture(puzzle, -app->render->camera.x + 70, -app->render->camera.y + 70);


	//menu pause
	if (paused) 
	{
		app->render->DrawTexture(pause, -app->render->camera.x + 391, -app->render->camera.y + 100);

		btnResume->Draw();
		btnSettings->Draw();
		btnBackIntro->Draw();
		btnExit->Draw();

	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(background);
	app->tex->UnLoad(heart);
	app->tex->UnLoad(key);
	app->tex->UnLoad(puzzle);
	app->tex->UnLoad(pause);
	app->entityManager->DestroyEntity(player);
	app->entityManager->DestroyEntity(enemy);
	app->entityManager->DestroyEntity(flyingEnemy);
	app->entityManager->DestroyEntity(particles);
	delete btnResume;
	//delete btnSettings;
	//delete btnBackIntro;
	//delete btnExit;
	//delete btnBackSettings;

	return true;
}



bool Scene::LoadState(pugi::xml_node& node)
{
	bool ret = true;
	app->scene->player->position.x = node.child("positionPlayer").attribute("x").as_int();
	app->scene->player->position.y = node.child("positionPlayer").attribute("y").as_int();
	return ret;
}

bool Scene::SaveState(pugi::xml_node& node) const
{
	bool ret = true;
	pugi::xml_node pnode = node.append_child("positionPlayer");
	pnode.append_attribute("x") = app->scene->player->position.x;
	pnode.append_attribute("y") = app->scene->player->position.y;
	return ret;
}

void Scene::Pause()
{
	app->tex->UnLoad(app->scene->player->texPlayer);
	app->tex->UnLoad(app->scene->enemy->texEnemy);
	app->tex->UnLoad(app->scene->flyingEnemy->texFlyingEnemy);
	app->tex->UnLoad(app->scene->particles->texture);


	//SDL_Rect rect = { -app->render->camera.x + 450, -app->render->camera.y + 50, 400, 450 };

	//Buttons
	btnResume = new GuiButton(1, { -app->render->camera.x+535, -app->render->camera.y+160, 210, 50 }, "RESUME");
	btnResume->SetObserver(this);

	btnSettings = new GuiButton(2, { -app->render->camera.x+505, -app->render->camera.y+263, 269, 51 }, "SETTINGS");
	btnSettings->SetObserver(this);

	btnBackIntro = new GuiButton(3, { -app->render->camera.x+485, -app->render->camera.y+368, 310, 50 }, "BACK MENU");
	btnBackIntro->SetObserver(this);

	btnExit = new GuiButton(4, { -app->render->camera.x+569, -app->render->camera.y+471, 145, 50 }, "EXIT");
	btnExit->SetObserver(this);

	if (pausedSettings)
	{
		btnBackSettings = new GuiButton(6, { -app->render->camera.x-500, -app->render->camera.y, 145, 50 }, "BACK");
		btnBackSettings->SetObserver(this);
	}

}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1)
		{
			paused = false;
			app->scene->player->texPlayer= app->tex->Load("Assets/Textures/player_textures.png");
			app->scene->enemy->texEnemy = app->tex->Load("Assets/Textures/enemy_texture.png");
			app->scene->flyingEnemy->texFlyingEnemy = app->tex->Load("Assets/Textures/flyingenemy_texture.png");
			app->scene->particles->texture = app->tex->Load("Assets/Textures/shot_fireball.png");
		}
		else if (control->id == 2)
		{
		}
		else if (control->id == 3)
		{
			app->fadetoblack->FadeToBlk(this, app->sceneIntro, 30);
			paused = false;
		}
		else if (control->id == 4)
		{
			if (app->scene->player != nullptr)
			{
				app->scene->player->position.x = 350;
				app->scene->player->position.y = 875;
				app->SaveGameRequest();

			}
			app->sceneIntro->posContinue = true;
			app->sceneIntro->exit = true;
		}
		else if (control->id == 5)
		{
			
		}
		else if (control->id == 6)
		{
			
		}
		else if (control->id == 7)
		{
			
		}
	}
	default: break;
	}

	return true;
}