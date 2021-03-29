#include <graphics/sprite_renderer.h>
#include <graphics/font.h>

#include "SplashState.h"
#include "MenuState.h"
#include "StateMachine.h"
#include "Level_1_State.h"
#include "PauseState.h"
#include "OptionsState.h"
#include "system/platform.h"

StateMachine::StateMachine(gef::Platform& platform_) : platform(platform_)
{
	currentState = nullptr;
	sprRend = nullptr;
	renderer3D = nullptr;
	font = nullptr;
	input_manager_ = nullptr;
	sc_ = nullptr;
	scim_ = nullptr;
	theWorld = nullptr;
}

StateMachine::~StateMachine()
{

}

void StateMachine::onEnter()
{
	currentState->onEnter();
}

void StateMachine::onExit()
{
	currentState->onExit();
}

void StateMachine::handleInput(float dt)
{
	currentState->handleInput(dt);
}

void StateMachine::init(gef::Platform& platform_, gef::SpriteRenderer* spRend, gef::Renderer3D* rend3D, gef::Font* font_, b2World* world)
{
	sprRend = spRend;
	renderer3D = rend3D;
	font = font_;
	theWorld = world;
}

void StateMachine::initInputControl(gef::InputManager* im, const gef::SonyController* sc, gef::SonyControllerInputManager* scim)
{
	input_manager_ = im;
	sc_ = sc;
	scim_ = scim;
}

bool StateMachine::update(float dt)
{
	return currentState->update(dt);
}

void StateMachine::render()
{
	currentState->render();
}

void StateMachine::initStates()
{
	splashState = new SplashState(this);
	mainMenuState = new MenuState(this);
	level_1_state = new Level_1_State(this);
	pauseState = new PauseState(this);
	optionsState = new OptionsState(this);

	currentState = splashState;
	initStateMap();
	onEnter();
}

void StateMachine::initStateMap()
{
	states.insert(std::make_pair("Splash", splashState));
	states.insert(std::make_pair("MainMenu", mainMenuState));
	states.insert(std::make_pair("Level_1", level_1_state));
	states.insert(std::make_pair("PauseMenu", pauseState));
	states.insert(std::make_pair("Options", optionsState));
}

void StateMachine::setState(std::string newState)
{
	onExit();
	currentState = states[newState];
	onEnter();
}

GameState* StateMachine::getCurrentState()
{
	return currentState;
}

gef::Platform& StateMachine::getPlatform()
{
	return platform;
}

gef::SpriteRenderer* StateMachine::getSpriteRenderer()
{
	return sprRend;
}

gef::Font* StateMachine::getFont()
{
	return font;
}

gef::InputManager* StateMachine::getInputManagaer()
{
	return input_manager_;
}

const gef::SonyController* StateMachine::getSonyController()
{
	return sc_;
}

gef::SonyControllerInputManager* StateMachine::getSonyControllerIM()
{
	return scim_;
}

b2World* StateMachine::getPhysicsWorld()
{
	return theWorld;
}

gef::Renderer3D* StateMachine::get3DRenderer()
{
	return renderer3D;
}