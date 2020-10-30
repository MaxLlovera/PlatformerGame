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
	position.x = 64;
	position.y = 875;

	upAnim.PushBack({ 0, 0, 64, 85 });
	//upAnim.PushBack({ 156, 310, 192, 256 });
	upAnim.speed = 0.1f;

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
	currentAnimation = &upAnim;

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speed;


		//collidatoL = false;
		//collider->SetPos(position.x, position.y);


	}
	else {

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{

			position.x += speed;


			//collidatoR = false;

			//collider->SetPos(position.x, position.y);

		}

	}

	//currentAnimation->Update();
	return true;
}

bool Player::PostUpdate()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texPlayer, position.x, position.y, &rect);
	return true;
}

bool Player::isWalkable(int x, int y)
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

bool Player::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
