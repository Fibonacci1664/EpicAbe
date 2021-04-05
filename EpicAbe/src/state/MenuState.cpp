#pragma once
#include "../texture/load_texture.h"
#include "MenuState.h"
#include "StateMachine.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>
#include <input/sony_controller_input_manager.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
MenuState::MenuState(StateMachine* sm)
{
	stateMachine = sm;
	stateGraphic = nullptr;
	isStartGame = false;
	isOptions = false;
	isHowToPlay = false;
}

MenuState::~MenuState()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void MenuState::onEnter()
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

void MenuState::onExit()
{
	isStartGame = false;
	isOptions = false;
	isHowToPlay = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void MenuState::handleInput(float dt)
{
	if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_CROSS)
	{
		isStartGame = true;	
	}
	else if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_TRIANGLE)
	{
		isOptions = true;
	}
	else if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_SQUARE)
	{
		isHowToPlay = true;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool MenuState::update(float dt)
{
	if (isStartGame)
	{
		stateMachine->setState("Level_1");
	}
	else if (isOptions)
	{
		stateMachine->setState("Options");
	}
	else if (isHowToPlay)
	{
		stateMachine->setState("HowToPlay");
	}

	return false;

	// THIS WILL BE WHERE COULD ALSO RETURN FALSE WHICH WILL QUIT THE GAME
	// SHOULD THE USE CHOOSE TO QUIT GAME FOLLOWING THE ON SCREEN PROMPTS.
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void MenuState::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	// Render button icon
	gef::Sprite mainMenu;
	mainMenu.set_texture(stateGraphic);
	mainMenu.set_position(stateMachine->getPlatform().width() * 0.5f, stateMachine->getPlatform().height() * 0.5f, -0.99f);
	mainMenu.set_height(1125.0f);
	mainMenu.set_width(2000.0f);
	stateMachine->getSpriteRenderer()->DrawSprite(mainMenu);

	stateMachine->getSpriteRenderer()->End();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Texture* MenuState::initStateGraphic(gef::Platform& platform_)
{
	stateGraphic = CreateTextureFromPNG("mainMenu.png", platform_);

	return stateGraphic;
}

/////////////////////////////////////////////////////////////////////////////////////////////////