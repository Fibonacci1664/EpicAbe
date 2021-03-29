#include <graphics/renderer_3d.h>
#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>
#include <graphics/font.h>
#include <input/sony_controller_input_manager.h>

#include "OptionsState.h"
#include "StateMachine.h"
#include "../texture/load_texture.h"


OptionsState::OptionsState(StateMachine* sm)
{
	stateMachine = sm;
	optionsGraphic = nullptr;
	stateTimer = 0;
	totalTimeElapsed = 0;
	moveToMainMenu = false;
}

OptionsState::~OptionsState()
{

}

void OptionsState::onEnter()
{
	if(optionsGraphic == nullptr)
	{
		initOptionsGraphic(stateMachine->getPlatform());
	}
}

void OptionsState::onExit()
{
	moveToMainMenu = false;
}

void OptionsState::handleInput(float dt)
{
	if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_CIRCLE)
	{
		moveToMainMenu = true;
	}
}

bool OptionsState::update(float dt)
{
	if (moveToMainMenu)
	{
		stateMachine->setState("MainMenu");
	}

	return false;
}

void OptionsState::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	// Render button icon
	gef::Sprite optionsMenu;
	optionsMenu.set_texture(optionsGraphic);
	optionsMenu.set_position(stateMachine->getPlatform().width() * 0.5f, stateMachine->getPlatform().height() * 0.5f, -0.99f);
	optionsMenu.set_height(1125.0f);
	optionsMenu.set_width(2000.0f);
	stateMachine->getSpriteRenderer()->DrawSprite(optionsMenu);

	stateMachine->getSpriteRenderer()->End();
}

gef::Texture* OptionsState::initOptionsGraphic(gef::Platform& platform_)
{
	optionsGraphic = CreateTextureFromPNG("optionsMenu.png", platform_);

	return optionsGraphic;
}