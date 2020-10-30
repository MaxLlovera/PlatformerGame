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

	void Jump();

	void gravityPlayer();

	iPoint position;




	Animation* currentAnimation = nullptr;

	Animation idlAnim;
	Animation rightAnim;
	Animation leftAnim;

	Animation rightAnim2;
	Animation leftAnim2;

	char opcio = 'l';

	Animation upAnim;
private:
	float speedX = 1.0f;
	float speedY = 0.0f;
	float gravity = 0.1f;


	SDL_Texture* texPlayer;
};

#endif // __PLAYER_H__