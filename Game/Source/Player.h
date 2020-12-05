#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"

struct SDL_Texture;


class Player : public Module
{
public:

	Player();

	// Destructor
	virtual ~Player();

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
	
	bool ThereIsChestBelow();
	bool ThereIsChestLeft();
	bool ThereIsChestRight();
	//bool ThereIsTopWall();
	bool ThereAreSpikes();
	bool ThereIsEnemy();
	bool ThereIsFlyingEnemy();
	bool TakeKey();
	bool TakePuzzle();
	bool TakeCheckpoint();
	bool TakeHeart();
	bool ThereIsDoor();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	void Jump();

	void GravityPlayer();

	bool loseLifes(); //when player loses a life
	
	iPoint position;


	//iPoint fireBallPos;



	Animation* currentAnimation = nullptr;

	Animation idlAnim;
	Animation rightAnim;
	Animation leftAnim;
	Animation jumpAnimRight;
	Animation jumpAnimLeft;
	Animation deathAnim;

	Animation upAnim;

	bool dead = false;
	bool win = false;
	bool godModeEnabled = false;
	
	int lifes = 3; //player lifes
	bool spiked; //if player lose a life

	int shotMaxCountdown = 20;
	int shotCountdown = 0;

private:
	float speedX = 3.0f;
	float speedY = 0.0f;
	float fireBallSpeed = 3.0f;
	float gravity = 0.15f;
	int playerHeight = 85;
	int playerWidth = 64;
	bool isJumping = false;
	int counterKey = 0;
	int counterCheckpoint = 0;
	int counterHeart = 0;
	SDL_Texture* texPlayer;
	SDL_Texture* texFireBall;
	uint playerDeathFx = 0;
	uint keyTakenFx = 0;
	uint checkpointFx = 0;
	uint heartFx = 0;
};

#endif // __PLAYER_H__