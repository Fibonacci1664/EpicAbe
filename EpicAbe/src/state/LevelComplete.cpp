/*
 * This class controls
 *		- Creating an image for the level complete screen.
 *		- Timing how long the level complete graphic displays for.
 *
 * Original @author D. Green.
 *
 * ? D. Green. 2021.
 */

/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StateMachine.h"
#include "LevelComplete.h"
#include "../texture/load_texture.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>
#include <input/sony_controller_input_manager.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
LevelComplete::LevelComplete(StateMachine* sm)
{
	stateMachine = sm;
	stateTimer = 0;
	stateGraphic = nullptr;
}

LevelComplete::~LevelComplete()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void LevelComplete::onEnter()
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
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void LevelComplete::onExit()
{
	stateTimer = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void LevelComplete::handleInput(float dt)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool LevelComplete::update(float dt)
{
	stateTimer += dt;

	// After 5 seconds return true, this will in turn pass back false to the main game
	// while loop and thus exit the game.
	if (stateTimer >= 5.0f)
	{
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void LevelComplete::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	gef::Sprite complete;
	complete.set_texture(stateGraphic);
	complete.set_position(stateMachine->getPlatform().width() * 0.5f, stateMachine->getPlatform().height() * 0.5f, -0.99f);
	complete.set_height(810);
	complete.set_width(1440);
	stateMachine->getSpriteRenderer()->DrawSprite(complete);

	stateMachine->getSpriteRenderer()->End();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Texture* LevelComplete::initStateGraphic(gef::Platform& platform_)
{
	stateGraphic = CreateTextureFromPNG("level_1_complete.png", platform_);

	return stateGraphic;
}

/////////////////////////////////////////////////////////////////////////////////////////////////