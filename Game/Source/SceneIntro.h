#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class GuiControl;
class GuiButton;

class SceneIntro : public Module
{
public:
	SceneIntro();
	~SceneIntro();

	// Called when the module is activated
	bool Awake(pugi::xml_node& config) override;
	// Loads the necessary textures for the map background
	bool Start();

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt) override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool OnGuiMouseClickEvent(GuiControl* control);


	bool CleanUp() override;

private:
	bool active = true;
	SString name;
	GuiButton* btnPlay;
	GuiButton* btnContinue;
	GuiButton* btnSettings;
	GuiButton* btnCredits;
	GuiButton* btnExit;
	GuiButton* btnBackSettings;
	GuiButton* btnBackCredits;

	SDL_Texture* introText = nullptr;

	int whiteFont = -1;

	bool settings = false;
	bool credits = false;

	bool exit = false;
};


#endif //__SCENE_INTRO_H__