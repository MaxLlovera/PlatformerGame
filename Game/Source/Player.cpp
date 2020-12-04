#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"


#define COLLIDER_GREEN 265
#define COLLIDER_RED 266
#define COLLIDER_BLUE 267
#define COLLIDER_YELLOW 268
#define COLLIDER_PINK 269
#define COLLIDER_GREY 270

Player::Player() : Module()
{
	name.Create("player");
	position.x = 350;
	position.y = 875;

	//idlanim
	idlAnim.PushBack({ 0, 0, 64, 85 });
	idlAnim.PushBack({ 0, 681, 64, 85 });
	idlAnim.speed = 0.02f;


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
		lifes = 3;
		counterKey = 0;
		counterCheckpoint = 0;
		counterHeart = 0;
		app->map->keyTaken = false;
		app->map->checkpointTaken = false;
		app->map->heartTaken = false;
		texPlayer = app->tex->Load("Assets/Textures/player_textures.png");
		playerDeathFx = app->audio->LoadFx("Assets/Audio/Fx/death_sound.wav");
		keyTakenFx = app->audio->LoadFx("Assets/Audio/Fx/key.wav");
		checkpointFx = app->audio->LoadFx("Assets/Audio/Fx/checkpoint.wav");
		heartFx = app->audio->LoadFx("Assets/Audio/Fx/heart.wav");
		currentAnimation = &idlAnim;
	}
	return true;
}

bool Player::Update(float dt)
{

	currentAnimation = &idlAnim;

	if ((ThereAreSpikes() || ThereIsEnemy() || ThereIsFlyingEnemy()) &&!spiked)
	{ 
		loseLifes();
		spiked = true;
	}
	
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
		if (counterKey == 0)
		{
			app->audio->PlayFx(keyTakenFx, 0);
		}
		counterKey = 1;
	}
	if (TakeCheckpoint()) {
		app->map->checkpointTaken = true;
		if (counterCheckpoint == 0) 
		{
			app->audio->PlayFx(checkpointFx, 0);
		}
		counterCheckpoint = 1;
	}
	if (TakeHeart()) {
		app->map->heartTaken = true;
		if (counterHeart == 0)
		{
			lifes++;
			app->audio->PlayFx(heartFx, 0);
		}
		counterHeart = 1;
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
//}

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

bool Player::ThereIsEnemy()
{
	bool valid = false;
	bool positionX = false;
	bool positionY = false;

	if (!godModeEnabled)
	{
		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				if (app->enemy->position.x + 16 + i == position.x + 16 + j) positionX = true;
			}
		}
		for (int i = 0; i < 62; i++)
		{
			for (int j = 0; j < 62; j++)
			{
				if (app->enemy->position.y + 22 + i == position.y + 22 + j) positionY = true;
			}
		}
	}
	if (positionX && positionY) valid = true;

	return valid;
}

bool Player::ThereIsFlyingEnemy()
{
	bool valid = false;
	bool positionX = false;
	bool positionY = false;

	if (!godModeEnabled)
	{
		for (int i = 0; i < 50; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				if (app->flyingEnemy->position.x + 6 + i == position.x + 16 + j) positionX = true;
			}
		}
		for (int i = 0; i < 42; i++)
		{
			for (int j = 0; j < 62; j++)
			{
				if (app->flyingEnemy->position.y + 4 + i == position.y + 22 + j) positionY = true;
			}
		}
	}
	if (positionX && positionY) valid = true;

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

bool Player::TakeCheckpoint()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int checkpoint;
		while (layer != NULL)
		{
			if (layer->data->properties.GetProperty("Navigation") == 0)
			{
				for (int i = 0; i < 3; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y + 21);
					checkpoint = layer->data->Get(tilePosition.x, tilePosition.y);
					if (checkpoint == COLLIDER_PINK) valid = true;
				}

			}
			layer = layer->next;
		}
	}
	return valid;

}

bool Player::TakeHeart()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int heart;
		while (layer != NULL)
		{
			if (layer->data->properties.GetProperty("Navigation") == 0)
			{
				for (int i = 0; i < 3; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y + 21);
					heart = layer->data->Get(tilePosition.x, tilePosition.y);
					if (heart == COLLIDER_GREY) valid = true;
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

bool Player::loseLifes()
{
	bool ret = false;

	lifes--;

	if (lifes == 0) dead = true;
	
	app->enemy->EnemyInitialPosition();
	app->flyingEnemy->FlyingEnemyInitialPosition();

	currentAnimation = &deathAnim;
	app->audio->PlayFx(playerDeathFx, 0);
	ret = true;
	spiked = false;


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