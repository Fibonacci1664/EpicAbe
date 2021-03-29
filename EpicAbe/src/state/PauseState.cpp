#include <graphics/renderer_3d.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <input/sony_controller_input_manager.h>

#include "PauseState.h"
#include "StateMachine.h"

PauseState::PauseState(StateMachine* sm)
{
	stateMachine = sm;
	stateTimer = 0;
	unpause = false;
}

PauseState::~PauseState()
{

}

void PauseState::onEnter()
{

}

void PauseState::onExit()
{
	stateTimer = 0;
	unpause = false;
}

void PauseState::handleInput(float dt)
{
	if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_SELECT)
	{
		unpause = true;		
	}
}

bool PauseState::update(float dt)
{
	stateTimer += dt;

	if (unpause)
	{
		stateMachine->setState("Level_1");
	}

	return false;
}

void PauseState::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	// render "TO START" text
	stateMachine->getFont()->RenderText(
		stateMachine->getSpriteRenderer(),
		gef::Vector4(stateMachine->getPlatform().width() * 0.5f, stateMachine->getPlatform().height() * 0.5f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"YOU PAUSED THE GAME!");

	stateMachine->getSpriteRenderer()->End();
}
