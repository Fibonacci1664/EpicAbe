/*
 * This class controls
 *		- Creating an image for the how to play screen.
 *		- Logic for button input.
 *		- Changing state based on button input.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2021.
 */

/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StateMachine.h"
#include "HowToPlay.h"
#include "../texture/load_texture.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>
#include <input/sony_controller_input_manager.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
HowToPlay::HowToPlay(StateMachine* sm)
{
	stateMachine = sm;
	stateGraphic = nullptr;
	back = false;
}

HowToPlay::~HowToPlay()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void HowToPlay::onEnter()
{
	/*
	 * Check if button icon is nullptr, if it is then load texture,
	 * if not then it's obv already been done, so don't do it again.
	 */
	if (stateGraphic == nullptr)
	{
		initStateGraphic(stateMachine->getPlatform());
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void HowToPlay::onExit()
{
	back = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void HowToPlay::handleInput(float dt)
{
	if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_CIRCLE)
	{
		back = true;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool HowToPlay::update(float dt)
{
	if (back)
	{
		stateMachine->setState("MainMenu");
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void HowToPlay::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	// Render button icon
	gef::Sprite howToPlay;
	howToPlay.set_texture(stateGraphic);
	howToPlay.set_position(stateMachine->getPlatform().width() * 0.5f, stateMachine->getPlatform().height() * 0.5f, -0.99f);
	howToPlay.set_height(810);
	howToPlay.set_width(1440);
	stateMachine->getSpriteRenderer()->DrawSprite(howToPlay);

	stateMachine->getSpriteRenderer()->End();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Texture* HowToPlay::initStateGraphic(gef::Platform& platform_)
{
	stateGraphic = CreateTextureFromPNG("howToPlay.png", platform_);

	return stateGraphic;
}

/////////////////////////////////////////////////////////////////////////////////////////////////