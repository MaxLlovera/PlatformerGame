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

	void GravityPlayer();

	bool IsDead();
	void FlyEnemyPosIni();

	iPoint position;

	Animation* currentAnimation = nullptr;

	Animation idlAnim;
	Animation rightAnim;
	Animation leftAnim;
	Animation deathAnim;
	Animation upAnim;

	bool dead = false;

private:
	float speedX = 3.0f;
	float speedY = 0.0f;
	float gravity = 0.15f;
	//int playerHeight = 85;
	//int playerWidth = 64;
	//bool isJumping = false;
	SDL_Texture* texFlyingEnemy;
	//uint playerDeathFx = 0;

};

#endif // __FLYINGENEMY_H__