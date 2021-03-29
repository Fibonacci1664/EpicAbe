#include <graphics/renderer_3d.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <graphics/sprite.h>

#include "../texture/load_texture.h"
#include "SplashState.h"
#include "StateMachine.h"

SplashState::SplashState(StateMachine* sm)
{
	stateMachine = sm;
	stateTimer = 0;
	splashLogo = nullptr;
}

SplashState::~SplashState()
{

}

void SplashState::onEnter()
{
	// Init stuff specific to the state.
	stateTimer = 0.0f;

	/*
	 * Check is button icon nullptr, if it is then load texture,
	 * if not then it's obv already been done, so don't do it again.
	 */
	if (splashLogo == nullptr)
	{
		initSplashLogo(stateMachine->getPlatform());
	}
}

void SplashState::onExit()
{
	stateTimer = 0;
}

void SplashState::handleInput(float dt)
{

}

bool SplashState::update(float dt)
{
	stateTimer += dt;

	if (stateTimer >= 5.0f)
	{
		stateMachine->setState("MainMenu");
	}

	return false;
}

void SplashState::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	//// render "TO START" text
	//stateMachine->getFont()->RenderText(
	//	stateMachine->getSpriteRenderer(),
	//	gef::Vector4(stateMachine->getPlatform().width() * 0.5f, stateMachine->getPlatform().height() * 0.5f, -0.99f),
	//	1.0f,
	//	0xffffffff,
	//	gef::TJ_CENTRE,
	//	"LOADING...HONEST");

	// Render button icon
	gef::Sprite logo;
	logo.set_texture(splashLogo);
	logo.set_position(stateMachine->getPlatform().width() * 0.5f, stateMachine->getPlatform().height() * 0.5f, -0.99f);
	logo.set_height(532.0f);
	logo.set_width(532.0f);
	stateMachine->getSpriteRenderer()->DrawSprite(logo);

	stateMachine->getSpriteRenderer()->End();
}

gef::Texture* SplashState::initSplashLogo(gef::Platform& platform_)
{
	splashLogo = CreateTextureFromPNG("Polymorphic.png", platform_);

	return splashLogo;
}
