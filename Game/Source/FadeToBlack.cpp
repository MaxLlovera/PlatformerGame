#include "FadeToBlack.h"


#include "App.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"

#include "Log.h"

FadeToBlack::FadeToBlack()
{

	name.Create("fadetoblack");

}


FadeToBlack::~FadeToBlack()
{

}


bool FadeToBlack::Awake()
{
	LOG("Loading FadeToBlack");
	bool ret = true;

	return ret;
}


bool FadeToBlack::Start()
{
	if (this->active == true)
	{
		uint w, h;
		app->win->GetWindowSize(w, h);

		screenRect = { 0, 0, (int)w * (int)app->win->GetScale(),  (int)h * (int)app->win->GetScale() };

		SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	}
	return true;
}




bool FadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			moduleToDisable->Disable();
			moduleToEnable->Enable();

			currentStep = Fade_Step::FROM_BLACK;
		}
	}
	else
	{
		--frameCount;

		if (frameCount <= 0) currentStep = Fade_Step::NONE;
	}

	return true;
}

bool FadeToBlack::PostUpdate()
{
	if (currentStep == Fade_Step::NONE) return true;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return true;
}


bool FadeToBlack::FadeToBlk(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;
		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;
		ret = true;
	}

	return ret;
}

bool FadeToBlack::CleanUp()
{
	return true;
}

