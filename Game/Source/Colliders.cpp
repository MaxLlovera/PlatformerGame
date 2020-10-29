#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Colliders.h"

#include "Defs.h"
#include "Log.h"


Colliders::Colliders() : Module()
{
	name.Create("collider");

}

Colliders::~Colliders()
{

}
