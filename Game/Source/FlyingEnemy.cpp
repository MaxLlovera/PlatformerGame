#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "FlyingEnemy.h"
#include "Pathfinding.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"


#define COLLIDER_GREEN 265
#define COLLIDER_RED 266
#define COLLIDER_BLUE 267
#define COLLIDER_YELLOW 268

FlyingEnemy::FlyingEnemy() : Module()
{
	name.Create("flyingenemy");
	position.x = 400;
	position.y = 1500;

	//idlanim
	idlAnim.PushBack({ 0, 0, 64, 52 });
	idlAnim.PushBack({ 0, 52, 64, 52 });
	idlAnim.speed = 0.03f;


	//move right
	moveAnim.PushBack({ 0, 52, 64, 52 });
	moveAnim.PushBack({ 0, 0, 64, 52 });
	moveAnim.speed = 0.1f;


}

// Destructor
FlyingEnemy::~FlyingEnemy()
{}

bool FlyingEnemy::Awake()
{
	LOG("Loading Flying Enemy");
	bool ret = true;

	return ret;
}

bool FlyingEnemy::Start()
{
	if (this->active == true)
	{
		dead = false;
		texFlyingEnemy = app->tex->Load("Assets/Textures/flyingenemy_texture.png");
		//playerDeathFx = app->audio->LoadFx("Assets/Audio/Fx/death_sound.wav");

		currentAnimation = &idlAnim;
	}
	return true;
}

bool FlyingEnemy::Update(float dt)
{

	currentAnimation = &idlAnim;

	if ((position.DistanceTo(app->player->position) < 500))
	{
		currentAnimation = &idlAnim;
		iPoint posOrigin;
		iPoint posDestination = app->player->position;

		posOrigin = app->map->WorldToMap(position.x+33, position.y+31);
		posDestination = app->map->WorldToMap(posDestination.x +64, posDestination.y+64);

		app->pathfinding->CreatePath(posOrigin, posDestination);
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

		if (path->At(1) != NULL)
		{
			if (path->At(1)->x < posOrigin.x /*&& ThereIsGroundLeft()*/)
			{
				position.x -= speedX;
			}
			if (path->At(1)->x > posOrigin.x /*&& ThereIsGroundRight()*/)
			{
				position.x += speedX;
			}
			if (path->At(1)->y < posOrigin.y /*&& ThereIsGroundRight()*/)
			{
				position.y -= speedX;
			}
			if (path->At(1)->y > posOrigin.y /*&& ThereIsGroundRight()*/)
			{
				position.y += speedX;
			}
		}
	}

	currentAnimation->Update();
	return true;
}

bool FlyingEnemy::PostUpdate()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texFlyingEnemy, position.x, position.y, &rect);
	return true;
}

bool FlyingEnemy::ThereIsGround()
{
	bool valid = false;
	//if (!godModeEnabled)
	//{
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 3; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y /*+ playerHeight*/);
				groundId = layer->data->Get(tilePosition.x, tilePosition.y);
				if (groundId == COLLIDER_RED) valid = true;
			}

		}
		layer = layer->next;
	}
	//}
	return valid;

}

bool FlyingEnemy::ThereIsLeftWall()
{
	bool valid = false;
	//if (!godModeEnabled)
	//{
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x, position.y + 21 + i * 16);
				groundId = layer->data->Get(tilePosition.x, tilePosition.y);
				if (groundId == COLLIDER_RED) valid = true;
			}
		}
		layer = layer->next;
	}
	//}
	return valid;

}

bool FlyingEnemy::ThereIsRightWall()
{
	bool valid = false;
	//if (!godModeEnabled)
	//{
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x /*+ playerWidth*/, position.y + 21 + i * 16);
				groundId = layer->data->Get(tilePosition.x, tilePosition.y);
				if (groundId == COLLIDER_RED) valid = true;
			}
		}
		layer = layer->next;
	}
	//}
	return valid;
}

void FlyingEnemy::GravityPlayer()
{
	if (!ThereIsGround())
	{
		speedY -= gravity;
		position.y -= speedY;
		//if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) currentAnimation = &jumpAnimLeft;
		//else currentAnimation = &jumpAnimRight;
	}
}

bool FlyingEnemy::IsDead()
{
	bool ret = false;

	dead = true;
	this->Disable();
	//app->audio->PlayFx(playerDeathFx, 0);
	ret = true;


	return ret;
}

bool FlyingEnemy::CleanUp()
{
	LOG("Freeing scene");
	//app->tex->UnLoad(texPlayer);
	return true;
}


bool FlyingEnemy::LoadState(pugi::xml_node& node)
{
	bool ret = true;
	position.x = node.child("positionFlyingEnemy").attribute("x").as_int();
	position.y = node.child("positionFlyingEnemy").attribute("y").as_int();
	return ret;
}

bool FlyingEnemy::SaveState(pugi::xml_node& node) const
{
	bool ret = true;
	pugi::xml_node pnode = node.append_child("positionFlyingEnemy");
	pnode.append_attribute("x") = position.x;
	pnode.append_attribute("y") = position.y;
	return ret;
}

void FlyingEnemy::FlyingEnemyInitialPosition()
{
	position.x = 400;
	position.y = 1500;
}
