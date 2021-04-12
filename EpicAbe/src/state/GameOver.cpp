/*
 * This class controls
 *		- Creating an image for the level 1 intro screen.
 *		- Initialising and playing game over music.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2021.
 */

/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StateMachine.h"
#include "GameOver.h"
#include "../texture/load_texture.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>
#include <input/sony_controller_input_manager.h>
#include <audio/audio_manager.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
GameOver::GameOver(StateMachine* sm)
{
	stateMachine = sm;
	stateTimer = 0;
	stateGraphic = nullptr;
}

GameOver::~GameOver()
{
	delete gameOverMusicVolinfo;
	gameOverMusicVolinfo = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void GameOver::onEnter()
{
	// Init stuff specific to the state.
	stateTimer = 0.0f;

	/*
	 * Check is button icon nullptr, if it is then load texture,
	 * if not then it's obv already been done, so don't do it again.
	 */
	if (stateGraphic == nullptr)
	{
		initStateGraphic(stateMachine->getPlatform());
	}

	if (audioManager == nullptr)
	{
		initStateAudio();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void GameOver::onExit()
{
	stateTimer = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void GameOver::handleInput(float dt)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool GameOver::update(float dt)
{
	stateTimer += dt;

	if (stateTimer >= 10.0f)
	{
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void GameOver::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	gef::Sprite gameOver;
	gameOver.set_texture(stateGraphic);
	gameOver.set_position(stateMachine->getPlatform().width() * 0.5f, stateMachine->getPlatform().height() * 0.5f, -0.99f);
	gameOver.set_height(810);
	gameOver.set_width(1440);
	stateMachine->getSpriteRenderer()->DrawSprite(gameOver);

	stateMachine->getSpriteRenderer()->End();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Texture* GameOver::initStateGraphic(gef::Platform& platform_)
{
	stateGraphic = CreateTextureFromPNG("gameOver.png", platform_);

	return stateGraphic;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void GameOver::initStateAudio()
{
	audioManager = stateMachine->getAudioManager();
	audioManager->StopMusic();

	gameOverMusic = audioManager->LoadMusic("anubis.wav", stateMachine->getPlatform());

	gameOverMusicVolinfo = new gef::VolumeInfo();
	gameOverMusicVolinfo->volume = 100;
	audioManager->SetMusicVolumeInfo(*gameOverMusicVolinfo);

	audioManager->PlayMusic();
}

/////////////////////////////////////////////////////////////////////////////////////////////////