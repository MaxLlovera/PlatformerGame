#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"

struct SDL_Texture;

class SceneGameplay : public Scene
{
public:

	SceneGameplay();

	// Destructor
	virtual ~SceneGameplay();

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

private:
	SDL_Texture* background;
	int volume;
	SDL_Texture* debugTex;
	SDL_Texture* heart;
	SDL_Texture* key;
	SDL_Texture* puzzle;
};

#endif // __SCENEGAMEPLAY_H__