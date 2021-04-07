#pragma once
#include "StateMachine.h"
#include "Level_1_Intro.h"
#include "../texture/load_texture.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/sprite_renderer.h>
#include <graphics/sprite.h>
#include <input/sony_controller_input_manager.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
Level_1_Intro::Level_1_Intro(StateMachine* sm)
{
	stateMachine = sm;
	stateTimer = 0;
	stateGraphic = nullptr;
}

Level_1_Intro::~Level_1_Intro()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void Level_1_Intro::onEnter()
{
	// Init stuff specific to the state.
	stateTimer = 0.0f;

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

void Level_1_Intro::onExit()
{
	stateTimer = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_Intro::handleInput(float dt)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool Level_1_Intro::update(float dt)
{
	stateTimer += dt;

	if (stateTimer >= 5.0f)
	{
		stateMachine->setState("Level_1");
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_Intro::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	gef::Sprite intro;
	intro.set_texture(stateGraphic);
	intro.set_position(stateMachine->getPlatform().width() * 0.5f, stateMachine->getPlatform().height() * 0.5f, -0.99f);
	intro.set_height(810);
	intro.set_width(1440);
	stateMachine->getSpriteRenderer()->DrawSprite(intro);

	stateMachine->getSpriteRenderer()->End();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Texture* Level_1_Intro::initStateGraphic(gef::Platform& platform_)
{
	stateGraphic = CreateTextureFromPNG("level_1_intro.png", platform_);

	return stateGraphic;
}

/////////////////////////////////////////////////////////////////////////////////////////////////