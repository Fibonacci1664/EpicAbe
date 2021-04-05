#pragma once
#include "PauseState.h"
#include "StateMachine.h"
#include "../texture/load_texture.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>
#include <input/sony_controller_input_manager.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
PauseState::PauseState(StateMachine* sm)
{
	stateMachine = sm;
	stateGraphic = nullptr;
	stateTimer = 0;
	unpause = false;
	gotoMainMenu = false;
	gotoOptions = false;
}

PauseState::~PauseState()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void PauseState::onEnter()
{
	if (stateGraphic == nullptr)
	{
		initStateGraphic(stateMachine->getPlatform());
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void PauseState::onExit()
{
	stateTimer = 0;
	unpause = false;
	gotoMainMenu = false;
	gotoOptions = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void PauseState::handleInput(float dt)
{
	if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_CIRCLE)
	{
		unpause = true;		
	}
	else if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_SQUARE)
	{
		gotoMainMenu = true;
	}
	else if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_TRIANGLE)
	{
		gotoOptions = true;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool PauseState::update(float dt)
{
	stateTimer += dt;

	if (unpause)
	{
		stateMachine->setState("Level_1");
	}
	else if (gotoMainMenu)
	{
		stateMachine->setState("MainMenu");
	}
	else if (gotoOptions)
	{
		stateMachine->setState("Options");
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void PauseState::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	gef::Sprite pauseMenu;
	pauseMenu.set_texture(stateGraphic);
	pauseMenu.set_position(stateMachine->getPlatform().width() * 0.5f, stateMachine->getPlatform().height() * 0.5f, -0.99f);
	pauseMenu.set_height(1125.0f);
	pauseMenu.set_width(2000.0f);
	stateMachine->getSpriteRenderer()->DrawSprite(pauseMenu);

	stateMachine->getSpriteRenderer()->End();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Texture* PauseState::initStateGraphic(gef::Platform& platform_)
{
	stateGraphic = CreateTextureFromPNG("pauseMenu.png", platform_);

	return stateGraphic;
}

/////////////////////////////////////////////////////////////////////////////////////////////////