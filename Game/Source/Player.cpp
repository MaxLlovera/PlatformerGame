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

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		
		if(!thereIsLeftWall())
		{
			position.x -= speedX;
			currentAnimation = &leftAnim;
		}
		//collidatoL = false;
		//collider->SetPos(position.x, position.y);


	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (!thereIsRightWall())
		{
			position.x += speedX;
			currentAnimation = &rightAnim;
		}
			//collidatoR = false;

			//collider->SetPos(position.x, position.y);

	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && thereIsGround())
	{
		isJumping = true;
		speedY = 10.0f;
		//gravityPlayer();
	}
	if (isJumping) 
	{
		Jump();
		isJumping = false;
	}
	gravityPlayer();
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
	iPoint tilePosition = app->map->WorldToMap(position.x, position.y + playerheight);
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL) 
	{
		if (layer->data->properties.GetProperty("Navigation") == 0) 
		{
			groundId = layer->data->Get(tilePosition.x, tilePosition.y);
			if (groundId == 266) {
				valid = true;
			}
		}
		layer = layer->next;
	}
	return valid;

}

bool Player::thereIsLeftWall()
{
	bool valid = false;
	iPoint tilePosition = app->map->WorldToMap(position.x, position.y);
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL) 
	{
		if (layer->data->properties.GetProperty("Navigation") == 0) 
		{
			groundId = layer->data->Get(tilePosition.x, tilePosition.y);
			if (groundId == 266) {
				valid = true;
			}
		}
		layer = layer->next;
	}
	return valid;

}

bool Player::thereIsRightWall()
{
	bool valid = false;
	iPoint tilePosition = app->map->WorldToMap(position.x + playerwidth, position.y);
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL) 
	{
		if (layer->data->properties.GetProperty("Navigation") == 0) 
		{
			groundId = layer->data->Get(tilePosition.x, tilePosition.y);
			if (groundId == 266) {
				valid = true;
			}
		}
		layer = layer->next;
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
		
	}
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