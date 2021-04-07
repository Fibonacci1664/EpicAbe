#pragma once
#include "StateMachine.h"
#include "OptionsState.h"
#include "../texture/load_texture.h"
#include "../../Game.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>
#include <input/sony_controller_input_manager.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
OptionsState::OptionsState(StateMachine* sm)
{
	stateMachine = sm;
	stateGraphic = nullptr;
	stateTimer = 0;
	totalTimeElapsed = 0;
	moveToMainMenu = false;
}

OptionsState::~OptionsState()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void OptionsState::onEnter()
{
	if(stateGraphic == nullptr)
	{
		initStateGraphic(stateMachine->getPlatform());
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void OptionsState::onExit()
{
	moveToMainMenu = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void OptionsState::handleInput(float dt)
{
	if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_CIRCLE)
	{
		moveToMainMenu = true;
	}

	if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_L1)
	{
		// turn music down
		float volume = stateMachine->getGame()->getGameVolume();
		volume *= 0.75f;
		stateMachine->getGame()->setGameVolume(volume);
	}
	else if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_R1)
	{
		// turn music up
		float volume = stateMachine->getGame()->getGameVolume();
		volume *= 1.25f;
		stateMachine->getGame()->setGameVolume(volume);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool OptionsState::update(float dt)
{
	if (moveToMainMenu)
	{
		stateMachine->setState("MainMenu");
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void OptionsState::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	// Render button icon
	gef::Sprite optionsMenu;
	optionsMenu.set_texture(stateGraphic);
	optionsMenu.set_position(stateMachine->getPlatform().width() * 0.5f, stateMachine->getPlatform().height() * 0.5f, -0.99f);
	optionsMenu.set_height(810);
	optionsMenu.set_width(1440);
	stateMachine->getSpriteRenderer()->DrawSprite(optionsMenu);

	stateMachine->getSpriteRenderer()->End();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Texture* OptionsState::initStateGraphic(gef::Platform& platform_)
{
	stateGraphic = CreateTextureFromPNG("optionsMenu.png", platform_);

	return stateGraphic;
}

/////////////////////////////////////////////////////////////////////////////////////////////////