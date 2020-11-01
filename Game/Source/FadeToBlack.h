#ifndef __FADETOBLACK_H__
#define __FADETOBLACK_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class FadeToBlack : public Module
{
public:

	FadeToBlack();
	virtual ~FadeToBlack();

	bool Awake();

	bool Start() override;


	bool Update();
	bool PostUpdate();

	bool Fadetoblack(Module* toDisable, Module* toEnable, float frames = 60);
	bool CleanUp();
private:

	enum Fade_Step
	{
		NONE,
		TO_BLACK,
		FROM_BLACK
	} currentStep = Fade_Step::NONE;

	// A frame count system to handle the fade time and ratio
	Uint32 frameCount = 0;
	Uint32 maxFadeFrames = 0;

	// The rectangle of the screen, used to render the black rectangle
	SDL_Rect screenRect;


	Module* moduleToEnable = nullptr;
	Module* moduleToDisable = nullptr;


};

















#endif //__MODULEFADETOBLACK_H__