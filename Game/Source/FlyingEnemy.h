#ifndef __FLYINGENEMY_H__
#define __FLYINGENEMY_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"

struct SDL_Texture;


class FlyingEnemy : public Module
{
public:

	FlyingEnemy();

	// Destructor
	virtual ~FlyingEnemy();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool ThereIsGround();
	bool ThereIsLeftWall();
	bool ThereIsRightWall();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;


	bool IsDead();
	void FlyingEnemyInitialPosition();

	iPoint position;

	Animation* currentAnimation = nullptr;

	Animation idlAnim;
	Animation deathAnim;

	bool dead = false;

private:
	float speed = 2.0f;
	SDL_Texture* texFlyingEnemy;
	uint flyingEnemyDeathFx = 0;

};

#endif // __FLYINGENEMY_H__