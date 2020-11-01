#include "SceneIntro.h"


#include "Log.h"

SceneIntro::SceneIntro()
{
	name.Create("sceneIntro");
}

SceneIntro::~SceneIntro()
{

}


bool SceneIntro::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	return ret;
}

bool SceneIntro::Update()
{
	//skyAnim.Update();
	return true;
}

// Update: draw background
bool SceneIntro::PostUpdate()
{
	return true;
}

bool SceneIntro::CleanUp()
{
	return true;
}