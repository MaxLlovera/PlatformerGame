#include "SceneIntro.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "SceneWin.h"
#include "SceneLose.h"
#include "FadeToBlack.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "Font.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

SceneIntro::SceneIntro() : Module()
{
	name.Create("SceneIntro");
}

SceneIntro::~SceneIntro()
{

}

bool SceneIntro::Awake(pugi::xml_node& node)
{
	return true;
}

bool SceneIntro::Start()
{
	LOG("Loading background assets");
	introText = app->tex->Load("Assets/Textures/scene_intro.png");
	creditText = app->tex->Load("Assets/Textures/credits.png");
	bool ret = true;
	app->sceneLose->Disable();
	app->sceneWin->Disable();
	app->audio->PlayMusic("Assets/Audio/Music/intro_theme.ogg");
	app->render->camera.x = 0;
	app->render->camera.y = -555;
	//app->sceneWin->wined = false;

	//app->scene->player->position.x = 350;
	//app->scene->player->position.y = 875;
	//if (app->scene->player->win) app->SaveGameRequest();
	//app->scene->player->win = false;
	char lookupTable[] = { "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ:/,!+-%  " };
	whiteFont = app->font->Load("Assets/Textures/white_font.png", lookupTable, 9);
	yellowFont = app->font->Load("Assets/Textures/yellow_font.png", lookupTable, 9);
	btnPlay = new GuiButton(1, { 540, 950, 145, 50 }, "PLAY");
	btnPlay->SetObserver(this);
	
	btnContinue = new GuiButton(2, { 480, 1025, 270, 50 }, "CONTINUE");
	btnContinue->SetObserver(this);
	
	btnSettings = new GuiButton(3, { 988, 1200, 270, 50 }, "SETTINGS");
	btnSettings->SetObserver(this);
	
	btnCredits = new GuiButton(4, { 19, 1200, 240, 50 }, "CREDITS");
	btnCredits->SetObserver(this);
	
	btnExit = new GuiButton(5, { 1113, 582, 145, 50 }, "EXIT");
	btnExit->SetObserver(this);
	
	btnBackSettings = new GuiButton(6, { 540, 1200, 145, 50 }, "BACK");
	btnBackSettings->SetObserver(this);
	
	btnBackCredits = new GuiButton(7, { 540, 1200, 145, 50 }, "BACK");
	btnBackCredits->SetObserver(this);

	btnControls = new GuiButton(8, { 480, 1200, 270, 50 }, "CONTROLS");
	btnControls->SetObserver(this);

	btnBackControls = new GuiButton(9, { 540, 1200, 145, 50 }, "BACK");
	btnBackControls->SetObserver(this);

	sliderMusicVolume = new GuiSlider(1, { 725, 950, 10, 28 }, "MUSIC VOLUME");
	sliderMusicVolume->SetObserver(this);
	
	sliderFxVolume = new GuiSlider(2, { 725, 1000, 10, 28 }, " FX VOLUME");
	sliderFxVolume->SetObserver(this);
	return ret;
}

bool SceneIntro::Update(float dt)
{
	if (settings == true)
	{
		sliderMusicVolume->Update(dt);
		sliderFxVolume->Update(dt);
		btnExit->Update(dt);
		btnBackSettings->Update(dt);
		btnSettings->Update(dt);
		btnCredits->Update(dt);
	}
	else if (credits == true)
	{
		btnExit->Update(dt);
		btnBackCredits->Update(dt);
		btnSettings->Update(dt);
		btnCredits->Update(dt);
	}
	
	else if (controls == true)
	{
		btnExit->Update(dt);
		btnBackControls->Update(dt);
		btnSettings->Update(dt);
		btnCredits->Update(dt);
	}
	else
	{
		btnPlay->Update(dt);
		btnContinue->Update(dt);
		btnSettings->Update(dt);
		btnCredits->Update(dt);
		btnExit->Update(dt);
		btnControls->Update(dt);
	}

	return true;
}

// Update: draw background
bool SceneIntro::PostUpdate()
{
	bool ret = true;
	if (exit == true) ret = false;

	app->render->DrawTexture(introText, 0, 555, NULL);

	if (settings == true)
	{
		app->render->DrawTexture(creditText, 220, 900, NULL);
		sliderMusicVolume->Draw();
		sliderFxVolume->Draw();
		btnBackSettings->Draw();
		btnExit->Draw();
		btnCredits->Draw();
		btnSettings->Draw();
	}
	else if (credits == true)
	{
		app->render->DrawTexture(creditText, 220, 900, NULL);
		app->font->DrawText(450, 380, yellowFont, "FAKE XEICS");
		app->font->DrawText(420, 450, whiteFont, "ARNAU BONADA");
		app->font->DrawText(430, 500, whiteFont, "MAX LLOVERA");
		app->font->DrawText(405, 550, whiteFont, "ARNAU USTRELL");
		btnBackCredits->Draw();
		btnExit->Draw();
		btnCredits->Draw();
		btnSettings->Draw();
	}
	
	else if (controls == true)
	{
		app->render->DrawTexture(creditText, 220, 900, NULL);
		app->font->DrawText(250, 380, whiteFont, "MOVE LEFT");
		app->font->DrawText(250, 420, whiteFont, "MOVE RIGHT");
		app->font->DrawText(250, 460, whiteFont, "JUMP");
		app->font->DrawText(250, 500, whiteFont, "SHOOT");
		app->font->DrawText(250, 540, whiteFont, "USE OBJECT");
		app->font->DrawText(700, 380, yellowFont, "A");
		app->font->DrawText(700, 420, yellowFont, "D");
		app->font->DrawText(700, 460, yellowFont, "SPACE");
		app->font->DrawText(700, 500, yellowFont, "P");
		app->font->DrawText(700, 540, yellowFont, "E");
		btnBackControls->Draw();
		btnExit->Draw();
		btnCredits->Draw();
		btnSettings->Draw();
	}
	else
	{
		btnPlay->Draw();
		btnContinue->Draw();
		if (!posContinue || app->sceneWin->won || app->sceneLose->lost) btnContinue->state = GuiControlState::DISABLED;
		else if (posContinue) btnContinue->state = GuiControlState::NORMAL;
		btnSettings->Draw();
		btnCredits->Draw();
		btnExit->Draw();
		btnControls->Draw();
	}
	

	return ret;
}

bool SceneIntro::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1)
		{
			app->fadetoblack->FadeToBlk(this, app->scene, 30);
			playClicked = true;
		}
		else if (control->id == 2)
		{
			app->fadetoblack->FadeToBlk(this, app->scene, 30);
		}
		else if (control->id == 3)
		{
			btnCredits->state = GuiControlState::DISABLED;
			btnSettings->state = GuiControlState::DISABLED;
			settings = true;
		}
		else if (control->id == 4)
		{
			btnCredits->state = GuiControlState::DISABLED;
			btnSettings->state = GuiControlState::DISABLED;
			credits = true;
		}
		else if (control->id == 5)
		{
			if (app->scene->player != nullptr)
			{
				app->scene->player->position.x = 350;
				app->scene->player->position.y = 875;
				app->SaveGameRequest();

			}
			exit = true;
		}
		else if (control->id == 6)
		{
			btnCredits->state = GuiControlState::NORMAL;
			btnSettings->state = GuiControlState::NORMAL;
			settings = false;
		}
		else if (control->id == 7)
		{
			btnCredits->state = GuiControlState::NORMAL;
			btnSettings->state = GuiControlState::NORMAL;
			credits = false;
		}
		else if (control->id == 8)
		{
			btnCredits->state = GuiControlState::DISABLED;
			btnSettings->state = GuiControlState::DISABLED;
			controls = true;
		}
		
		else if (control->id == 9)
		{
			btnCredits->state = GuiControlState::NORMAL;
			btnSettings->state = GuiControlState::NORMAL;
			controls = false;
		}
	}
	case GuiControlType::SLIDER:
	{
		if (control->id == 1) app->audio->ChangeMusicVolume(sliderMusicVolume->ReturnValue());
		else if (control->id == 2) app->audio->ChangeFxVolume(sliderFxVolume->ReturnValue());
		break; 
	}
	default: break;
	}

	return true;
}

bool SceneIntro::CleanUp()
{
	//delete btnPlay;
	//delete btnContinue;
	//delete btnSettings;
	//delete btnCredits;
	//delete btnExit;
	//delete btnBackSettings;
	//delete btnBackCredits;
	//delete sliderMusicVolume;
	//delete sliderFxVolume;

	app->tex->UnLoad(introText);
	return true;
}