#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Module.h"
#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class SceneIntro : public Module
{
public:
	SceneIntro();
	~SceneIntro();

	// Called when the module is activated
// Loads the necessary textures for the map background
	bool Start();

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update();

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();


	bool CleanUp() override;

private:
	SDL_Texture* bgTexture = nullptr;
	SDL_Texture* logo = nullptr;

	int whiteFont = -1;
	int blueFont = -1;
	char scoreText[10] = { "\0" };

};














#endif