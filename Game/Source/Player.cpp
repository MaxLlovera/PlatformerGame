#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

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
	texPlayer = app->tex->Load("Assets/textures/player_textures.png");

	return true;
}

bool Player::Update(float dt)
{

	currentAnimation = &idlAnim;

	if (thereAreSpikes()) 
	{
		isDead();
	}
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

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !thereAreSpikes())
	{
		
		if(!thereIsLeftWall())
		{
			position.x -= speedX;
			currentAnimation = &leftAnim;
		}
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !thereAreSpikes())
	{
		if (!thereIsRightWall())
		{
			position.x += speedX;
			currentAnimation = &rightAnim;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && thereIsGround() && !thereAreSpikes())
	{
		isJumping = true;
		speedY = 5.0f;
		//gravityPlayer();
	}
	if (isJumping) 
	{
		Jump();
		isJumping = false;
	}
	if (!godModeEnabled)
	{
		gravityPlayer();
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

bool Player::thereIsGround()
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
					tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y + playerheight);
					groundId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (groundId == 266) valid = true;
				}

			}
			layer = layer->next;
		}
	}
	return valid;

}

bool Player::thereIsLeftWall()
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
					if (groundId == 266) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;

}

bool Player::thereIsRightWall()
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
					tilePosition = app->map->WorldToMap(position.x + playerwidth, position.y + 21 + i * 16);
					groundId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (groundId == 266) valid = true;
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

bool Player::thereAreSpikes()
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
					if (groundId == 268) valid = true;
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

void Player::gravityPlayer()
{
	if (!thereIsGround())
	{
		speedY -= gravity;
		position.y -= speedY;
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) currentAnimation = &jumpAnimLeft;
		else currentAnimation = &jumpAnimRight;
	}
}

bool Player::isDead()
{
	bool ret = false;

	dead = true;
	currentAnimation = &deathAnim;
	ret = true;


	return ret;
}




bool Player::CleanUp()
{
	LOG("Freeing scene");

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