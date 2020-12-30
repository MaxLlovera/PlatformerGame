#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Scene.h"
#include "Animation.h"

struct SDL_Texture;

class SceneIntro : public Scene
{
public:
	SceneIntro();
	~SceneIntro();

	// Called when the module is activated
	bool Awake();
	// Loads the necessary textures for the map background
	bool Start();

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();


	bool CleanUp();

private:
	SDL_Texture* introText = nullptr;

};


#endif //__SCENE_INTRO_H__