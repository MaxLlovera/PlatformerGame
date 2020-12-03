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
	//bool ThereIsTopWall();
	bool ThereAreSpikes();
	bool ThereIsEnemy();
	bool TakeKey();
	bool TakeCheckpoint();
	bool ThereIsDoor();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	void Jump();

	void GravityPlayer();

	bool loseLifes(); //when player loses a life

	//bool IsDead(); //when player loses his last life
	

	iPoint position;




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

private:
	float speedX = 3.0f;
	float speedY = 0.0f;
	float gravity = 0.15f;
	int playerHeight = 85;
	int playerWidth = 64;
	bool isJumping = false;
	SDL_Texture* texPlayer;
	uint playerDeathFx = 0;
	
};

#endif // __PLAYER_H__