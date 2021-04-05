#pragma once
#include "OptionsState.h"
#include "StateMachine.h"
#include "../texture/load_texture.h"

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
	optionsMenu.set_height(1125.0f);
	optionsMenu.set_width(2000.0f);
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