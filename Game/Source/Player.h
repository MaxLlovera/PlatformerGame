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

	bool thereIsGround();
	bool thereIsLeftWall();
	bool thereIsRightWall();
	bool thereAreSpikes();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	void Jump();

	void gravityPlayer();

	bool isDead();

	iPoint position;




	Animation* currentAnimation = nullptr;

	Animation idlAnim;
	Animation rightAnim;
	Animation leftAnim;
	Animation jumpAnimRight;
	Animation jumpAnimLeft;
	Animation deathAnim;

	Animation rightAnim2;
	Animation leftAnim2;

	char opcio = 'l';
	bool dead = false;
	Animation upAnim;
private:
	float speedX = 3.0f;
	float speedY = 0.0f;
	float gravity = 0.15f;
	int playerheight = 85;
	int playerwidth = 64;
	bool isJumping = false;
	SDL_Texture* texPlayer;
	
};

#endif // __PLAYER_H__