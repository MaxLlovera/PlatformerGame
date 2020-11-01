#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"


#define COLLIDER_GREEN 265
#define COLLIDER_RED 266
#define COLLIDER_BLUE 267
#define COLLIDER_YELLOW 268


Player::Player() : Module()
{
	name.Create("player");
	position.x = 350;
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

	//jumpAnimRight
	jumpAnimRight.PushBack({ 0, 425, 64, 85 });
	//leftAnim.PushBack({ 0, 340, 64, 85 });
	jumpAnimRight.speed = 0.1f;
	
	//jumpAnimRight
	jumpAnimLeft.PushBack({ 0, 510, 64, 85 });
	//leftAnim.PushBack({ 0, 340, 64, 85 });
	jumpAnimLeft.speed = 0.1f;

	//deathAnim
	deathAnim.PushBack({ 0, 595, 64, 85 });
	//leftAnim.PushBack({ 0, 340, 64, 85 });
	deathAnim.speed = 0.1f;
}

// Destructor
Player::~Player()
{}

bool Player::Awake()
{
	LOG("Loading Player");
	bool ret = true;

	return ret;
}

bool Player::Start()
{
	if (this->active == true)
	{
		dead = false;
		win = false;
		texPlayer = app->tex->Load("Assets/textures/player_textures.png");
		playerDeathFx = app->audio->LoadFx("Assets/audio/fx/DeathSound.wav");
		
		currentAnimation = &idlAnim;
	}
	return true;
}

bool Player::Update(float dt)
{

	currentAnimation = &idlAnim;

	if (ThereAreSpikes()) IsDead();
	if (ThereIsDoor() && app->map->keyTaken) win = true;
	else
	{
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godModeEnabled)
		{
			position.y -= speedX;
			currentAnimation = &leftAnim;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godModeEnabled)
		{
			position.y += speedX;
			currentAnimation = &leftAnim;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !ThereAreSpikes())
		{
			if (!ThereIsLeftWall())
			{
				position.x -= speedX;
				currentAnimation = &leftAnim;
			}
		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !ThereAreSpikes())
		{
			if (!ThereIsRightWall())
			{
				position.x += speedX;
				currentAnimation = &rightAnim;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && ThereIsGround() && !ThereAreSpikes())
		{
			isJumping = true;
			speedY = 5.0f;
			//GravityPlayer();
		}
		if (isJumping)
		{
			Jump();
			isJumping = false;
		}
		if (!godModeEnabled)
		{
			GravityPlayer();
		}
	}
	

	if (TakeKey()) {
		app->map->keyTaken = true;
	}

	currentAnimation->Update();
	return true;
}

bool Player::PostUpdate()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texPlayer, position.x, position.y, &rect);
	return true;
}

bool Player::ThereIsGround()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int groundId;
		while (layer != NULL)
		{
			if (layer->data->properties.GetProperty("Navigation") == 0)
			{
				for (int i = 0; i < 3; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y + playerHeight);
					groundId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (groundId == COLLIDER_RED) valid = true;
				}

			}
			layer = layer->next;
		}
	}
	return valid;

}

bool Player::ThereIsLeftWall()
{
	bool valid = false;
	if (!godModeEnabled)
	{
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
	}
	return valid;

}

bool Player::ThereIsRightWall()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int groundId;
		while (layer != NULL)
		{
			if (layer->data->properties.GetProperty("Navigation") == 0)
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + playerWidth, position.y + 21 + i * 16);
					groundId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (groundId == COLLIDER_RED) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;
}

//bool Player::thereIsTopWall()
//{
//	bool valid = false;
//	if (!godModeEnabled)
//	{
//		iPoint tilePosition;
//		ListItem<MapLayer*>* layer = app->map->data.layers.start;
//		int groundId;
//		while (layer != NULL)
//		{
//			if (layer->data->properties.GetProperty("Navigation") == 0)
//			{
//				for (int i = 0; i < 4; ++i)
//				{
//					tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y+21);
//					groundId = layer->data->Get(tilePosition.x, tilePosition.y);
//					if (groundId == 266) valid = true;
//				}
//			}
//			layer = layer->next;
//		}
//	}
//	return valid;
//}º

bool Player::ThereAreSpikes()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int groundId;
		while (layer != NULL)
		{
			if (layer->data->properties.GetProperty("Navigation") == 0)
			{
				for (int i = 0; i < 3; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y + 21);
					groundId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (groundId == COLLIDER_YELLOW) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;
}

bool Player::TakeKey()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int key;
		while (layer != NULL)
		{
			if (layer->data->properties.GetProperty("Navigation") == 0)
			{
				for (int i = 0; i < 3; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y + 21);
					key = layer->data->Get(tilePosition.x, tilePosition.y);
					if (key == COLLIDER_BLUE) valid = true;
				}

			}
			layer = layer->next;
		}
	}
	return valid;

}

bool Player::ThereIsDoor()
{
	bool valid = false;
	if (!godModeEnabled)
	{
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
					if (groundId == COLLIDER_GREEN) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;

}


void Player::Jump() 
{
	speedY -= gravity;
	position.y -= speedY;
}

void Player::GravityPlayer()
{
	if (!ThereIsGround())
	{
		speedY -= gravity;
		position.y -= speedY;
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) currentAnimation = &jumpAnimLeft;
		else currentAnimation = &jumpAnimRight;
	}
}

bool Player::IsDead()
{
	bool ret = false;

	dead = true;
	currentAnimation = &deathAnim;
	app->audio->PlayFx(playerDeathFx,0);
	ret = true;
	

	return ret;
}

bool Player::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texPlayer);
	return true;
}


bool Player::LoadState(pugi::xml_node& node)
{
	bool ret = true;
	position.x = node.child("positionPlayer").attribute("x").as_int();
	position.y = node.child("positionPlayer").attribute("y").as_int();
	return ret;
}

bool Player::SaveState(pugi::xml_node& node) const
{
	bool ret = true;
	pugi::xml_node pnode = node.append_child("positionPlayer");
	pnode.append_attribute("x") = position.x;
	pnode.append_attribute("y") = position.y;
	return ret;
}