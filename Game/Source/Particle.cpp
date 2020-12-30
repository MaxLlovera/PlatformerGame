#include "Particle.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "ModuleParticles.h"
#include "FlyingEnemy.h"
#include "Defs.h"
#include "Log.h"

#define COLLIDER_RED 266

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) : anim(p.anim), position(p.position), speed(p.speed), frameCount(p.frameCount), lifetime(p.lifetime)
{

}

Particle::~Particle()
{

}

bool Particle::Update()
{
	bool ret = true;
	frameCount++;

	// The particle is set to 'alive' when the delay has been reached
	if (!isAlive && frameCount >= 0) isAlive = true;

	if (isAlive)
	{
		anim.Update();

		// If the particle has a specific lifetime, check when it has to be destroyed
		if (lifetime > 0)
		{
			if (frameCount >= lifetime) ret = false;
		}
		// Otherwise the particle is destroyed when the animation is finished
		else if (anim.HasFinished()) ret = false;

		// Update the position in the screen
		position.x += speed.x;
		position.y += speed.y;

	}
	if (ThereIsEnemy())
	{
		if (!app->enemy->dead)
		{
			app->enemy->IsDead();
			SetToDelete();
		}

	}
	if (ThereIsFlyingEnemy())
	{
		if (!app->flyingEnemy->dead)
		{
			app->flyingEnemy->IsDead();
			SetToDelete();
		}
	}
	if (ThereIsLeftWall())
	{
		SetToDelete();
	}
	if (ThereIsRightWall())
	{
		SetToDelete();
	}
	return ret;
}

void Particle::SetToDelete()
{

	pendingToDelete = true;

}

bool Particle::ThereIsEnemy()
{
	bool valid = false;
	bool positionX = false;
	bool positionY = false;

	for (int i = 0; i < 30; ++i)
	{
		for (int j = 0; j < 22; ++j)
		{
			if (app->enemy->position.x + 16 + i == position.x + j) positionX = true;
		}
	}
	for (int i = 0; i < 62; ++i)
	{
		for (int j = 0; j < 14; ++j)
		{
			if (app->enemy->position.y + 22 + i == position.y + j) positionY = true;
		}
	}

	if (positionX && positionY) valid = true;

	return valid;
}

bool Particle::ThereIsFlyingEnemy()
{
	bool valid = false;
	bool positionX = false;
	bool positionY = false;

	for (int i = 0; i < 60; ++i)
	{
		for (int j = 0; j < 22; ++j)
		{
			if (app->flyingEnemy->position.x + i == position.x + j) positionX = true;
		}
	}
	for (int i = 0; i < 50; ++i)
	{
		for (int j = 0; j < 14; ++j)
		{
			if (app->flyingEnemy->position.y + i == position.y + j) positionY = true;
		}
	}

	if (positionX && positionY) valid = true;

	return valid;
}

bool Particle::ThereIsLeftWall()
{
	bool valid = false;

	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x, position.y);
				groundId = layer->data->Get(tilePosition.x, tilePosition.y);
				if (groundId == COLLIDER_RED) valid = true;
			}
		}
		layer = layer->next;
	}

	return valid;

}

bool Particle::ThereIsRightWall()
{
	bool valid = false;

	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL)
	{
		for (int i = 0; i < 4; ++i)
		{
			tilePosition = app->map->WorldToMap(position.x+25, position.y);
			groundId = layer->data->Get(tilePosition.x, tilePosition.y);
			if (groundId == COLLIDER_RED) valid = true;
		}
		layer = layer->next;
	}

	return valid;
}