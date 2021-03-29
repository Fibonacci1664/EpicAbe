#include <graphics/renderer_3d.h>
#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>
#include <graphics/font.h>
#include <input/sony_controller_input_manager.h>
#include <graphics/colour.cpp>

#include "../texture/load_texture.h"
#include "MenuState.h"
#include "StateMachine.h"


MenuState::MenuState(StateMachine* sm)
{
	stateMachine = sm;
	mainMenuGraphic = nullptr;
	isStartGame = false;
	isOptions = false;
}

MenuState::~MenuState()
{

}

void MenuState::onEnter()
{
	/*
	 * Check is button icon nullptr, if it is then load texture,
	 * if not then it's obv already been done, so don't do it again.
	 */
	if (mainMenuGraphic == nullptr)
	{
		initMenuGraphic(stateMachine->getPlatform());
	}
}

void MenuState::onExit()
{
	isStartGame = false;
	isOptions = false;
}

void MenuState::handleInput(float dt)
{
	if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_CROSS)
	{
		isStartGame = true;	
	}

	if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_TRIANGLE)
	{
		isOptions = true;
	}
}

bool  MenuState::update(float dt)
{
	if (isStartGame)
	{
		stateMachine->setState("Level_1");
	}
	else if (isOptions)
	{
		stateMachine->setState("Options");
	}

	return false;

	// THIS WILL BE WEHRE COULD ALSO RETURN FALSE WHICH WILL QUIT THE GAME
	// SHOULD THE USE CHOOSE TO QUIT GAME FOLLOWING THE ON SCREEN PROMPTS.
}

void MenuState::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	// Render button icon
	gef::Sprite mainMenu;
	mainMenu.set_texture(mainMenuGraphic);
	mainMenu.set_position(stateMachine->getPlatform().width() * 0.5f, stateMachine->getPlatform().height() * 0.5f, -0.99f);
	mainMenu.set_height(1125.0f);
	mainMenu.set_width(2000.0f);
	stateMachine->getSpriteRenderer()->DrawSprite(mainMenu);

	stateMachine->getSpriteRenderer()->End();
}

gef::Texture* MenuState::initMenuGraphic(gef::Platform& platform_)
{
	mainMenuGraphic = CreateTextureFromPNG("mainMenu.png", platform_);

	return mainMenuGraphic;
}
