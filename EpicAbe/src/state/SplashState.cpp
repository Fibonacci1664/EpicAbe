/*
 * This class controls
 *		- Creating an image for the splash screen.
 *		- Timing how long the spalsh screen displays for.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2021.
 */

/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../texture/load_texture.h"
#include "StateMachine.h"
#include "SplashState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>
#include <input/sony_controller_input_manager.h>


/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
SplashState::SplashState(StateMachine* sm)
{
	stateMachine = sm;
	stateTimer = 0;
	stateGraphic = nullptr;
}

SplashState::~SplashState()
{
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void SplashState::onEnter()
{
	// Init stuff specific to the state.
	stateTimer = 0.0f;

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

void SplashState::onExit()
{
	stateTimer = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void SplashState::handleInput(float dt)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool SplashState::update(float dt)
{
	stateTimer += dt;

	// After 5 seconds switch to the main menu.
	if (stateTimer >= 5.0f)
	{
		stateMachine->setState("MainMenu");
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void SplashState::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	gef::Sprite logo;
	logo.set_texture(stateGraphic);
	logo.set_position(stateMachine->getPlatform().width() * 0.5f, stateMachine->getPlatform().height() * 0.5f, -0.99f);
	logo.set_height(532.0f);
	logo.set_width(532.0f);
	stateMachine->getSpriteRenderer()->DrawSprite(logo);

	stateMachine->getSpriteRenderer()->End();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Texture* SplashState::initStateGraphic(gef::Platform& platform_)
{
	stateGraphic = CreateTextureFromPNG("Polymorphic.png", platform_);

	return stateGraphic;
}

/////////////////////////////////////////////////////////////////////////////////////////////////