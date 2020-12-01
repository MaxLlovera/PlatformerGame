#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"


#define COLLIDER_GREEN 265
#define COLLIDER_RED 266
#define COLLIDER_BLUE 267
#define COLLIDER_YELLOW 268


Enemy::Enemy() : Module()
{
	name.Create("enemy");
	position.x = 400;
	position.y = 875;

	//idlanim
	idlAnim.PushBack({ 0, 0, 64, 85 });
	//idlAnim.PushBack({ 156, 310, 64, 85 });
	idlAnim.speed = 0.1f;


	//move right
	rightAnim.PushBack({ 0, 85, 64, 85 });
	rightAnim.PushBack({ 0, 170, 64, 85 });
	rightAnim.speed = 0.1f;

	//move left
	leftAnim.PushBack({ 0, 255, 64, 85 });
	leftAnim.PushBack({ 0, 340, 64, 85 });
	leftAnim.speed = 0.1f;


}

// Destructor
Enemy::~Enemy()
{}

bool Enemy::Awake()
{
	LOG("Loading Enemy");
	bool ret = true;

	return ret;
}

bool Enemy::Start()
{
	if (this->active == true)
	{
		dead = false;
		texEnemy = app->tex->Load("Assets/textures/enemy_texture.png");
		//playerDeathFx = app->audio->LoadFx("Assets/audio/fx/DeathSound.wav");

		currentAnimation = &idlAnim;
	}
	return true;
}

bool Enemy::Update(float dt)
{

	currentAnimation = &idlAnim;

	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
	{
		position.y -= speedX;
		currentAnimation = &leftAnim;
	}
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	{
		position.y += speedX;
		currentAnimation = &leftAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
	{
		if (!ThereIsLeftWall())
		{
			position.x -= speedX;
			currentAnimation = &leftAnim;
		}
	}
	else if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
	{
		if (!ThereIsRightWall())
		{
			position.x += speedX;
			currentAnimation = &rightAnim;
		}
	}

	currentAnimation->Update();
	return true;
}

bool Enemy::PostUpdate()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texEnemy, position.x, position.y, &rect);
	return true;
}

bool Enemy::ThereIsGround()
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

bool Enemy::ThereIsLeftWall()
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

bool Enemy::ThereIsRightWall()
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

void Enemy::GravityPlayer()
{
	if (!ThereIsGround())
	{
		speedY -= gravity;
		position.y -= speedY;
		//if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) currentAnimation = &jumpAnimLeft;
		//else currentAnimation = &jumpAnimRight;
	}
}

bool Enemy::IsDead()
{
	bool ret = false;

	dead = true;
	currentAnimation = &deathAnim;
	//app->audio->PlayFx(playerDeathFx, 0);
	ret = true;


	return ret;
}

bool Enemy::CleanUp()
{
	LOG("Freeing scene");
	//app->tex->UnLoad(texPlayer);
	return true;
}


bool Enemy::LoadState(pugi::xml_node& node)
{
	bool ret = true;
	position.x = node.child("positionPlayer").attribute("x").as_int();
	position.y = node.child("positionPlayer").attribute("y").as_int();
	return ret;
}

bool Enemy::SaveState(pugi::xml_node& node) const
{
	bool ret = true;
	pugi::xml_node pnode = node.append_child("positionPlayer");
	pnode.append_attribute("x") = position.x;
	pnode.append_attribute("y") = position.y;
	return ret;
}