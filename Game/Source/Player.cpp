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
	position.y = 896;

	upAnim.PushBack({ 0, 0, 64, 38 });
	//upAnim.PushBack({ 64, 0, 64, 64 });
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

bool Player::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
