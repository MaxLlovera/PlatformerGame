#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Colliders.h"

#include "Defs.h"
#include "Log.h"


Colliders::Colliders() : Module()
{
	name.Create("collider");

}

Colliders::~Colliders()
{

}


void Colliders::SetPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

bool Colliders::Intersects(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}


bool Colliders::Awake()
{
	LOG("Loading Collider");
	bool ret = true;

	return ret;
}

bool Colliders::Start()
{


	return true;
}

bool Colliders::Update(float dt)
{

	return true;
}

bool Colliders::PostUpdate()
{

	return true;
}

bool Colliders::CleanUp()
{
	LOG("Freeing colliders");

	return true;
}
