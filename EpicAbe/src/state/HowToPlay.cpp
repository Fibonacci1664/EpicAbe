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
	 * Check is button icon nullptr, if it is then load texture,
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

	// THIS WILL BE WEHRE COULD ALSO RETURN FALSE WHICH WILL QUIT THE GAME
	// SHOULD THE USE CHOOSE TO QUIT GAME FOLLOWING THE ON SCREEN PROMPTS.
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void HowToPlay::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	// Render button icon
	gef::Sprite howToPlay;
	howToPlay.set_texture(stateGraphic);
	howToPlay.set_position(stateMachine->getPlatform().width() * 0.5f, stateMachine->getPlatform().height() * 0.5f, -0.99f);
	howToPlay.set_height(1125.0f);
	howToPlay.set_width(2000.0f);
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