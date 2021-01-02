#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;

class Player;
class Enemy;
class FlyingEnemy;
class ModuleParticles;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//virtual bool OnGuiMouseClickEvent(GuiControl* control)
	//{
	//	return true;
	//}

	Player* player;
	Enemy* enemy;
	FlyingEnemy* flyingEnemy;
	ModuleParticles* particles;
private:
	SDL_Texture* background;
	int volume;
	SDL_Texture* debugTex;
	SDL_Texture* heart;
	SDL_Texture* key;
	SDL_Texture* puzzle;
};

#endif // __SCENE_H__