#ifndef __SCENE_H__
#define __SCENE_H__

#include "SString.h"

class Input;
class Render;
class Textures;

<<<<<<< HEAD
class Player;
class Enemy;
class FlyingEnemy;

class Scene : public Module
=======
class GuiControl;

enum class SceneType
{
    LOGO,
    INTRO,
    GAMEPLAY,
    ENDING
};

class Scene
>>>>>>> 781c6d6d2cec4b864a1563164947fa2745f6f153
{
public:

    Scene() : active(true), loaded(false), transitionRequired(false) {}

    virtual bool Load(Textures* tex)
    {
        return true;
    }

    virtual bool Update(Input* input, float dt)
    {
        return true;
    }

    virtual bool Draw(Render* render)
    {
        return true;
    }

    virtual bool Unload()
    {
        return true;
    }

    void TransitionToScene(SceneType scene)
    {
        transitionRequired = true;
        nextScene = scene;
    }

    // Define multiple Gui Event methods
    virtual bool OnGuiMouseClickEvent(GuiControl* control)
    {
        return true;
    }

public:

    bool active = true;
    SString name;         // Scene name identifier?

    // Possible properties
    bool loaded = false;
    // TODO: Transition animation properties

<<<<<<< HEAD

	Player* player;
	Enemy* enemy;
	FlyingEnemy* flyingEnemy;

private:
	SDL_Texture* background;
	int volume;
	SDL_Texture* debugTex;
	SDL_Texture* heart;
	SDL_Texture* key;
	SDL_Texture* puzzle;
=======
    bool transitionRequired;
    SceneType nextScene;
>>>>>>> 781c6d6d2cec4b864a1563164947fa2745f6f153
};

#endif // __SCENE_H__