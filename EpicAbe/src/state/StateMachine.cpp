/*
 * This class knows about everything the Game class knows about, namely:
 *		- Platfrom, Input, Audio etc.
 * 
 * This is so it can pass these by pointer to the respective states
 * as and when the states need to use them.
 * 
 * This class also initialises all the game states and stores them in a map
 * using the a string as the key.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2021.
 */

/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StateMachine.h"
#include "SplashState.h"
#include "MenuState.h"
#include "Level_1_Intro.h"
#include "Level_1_State.h"
#include "LevelComplete.h"
#include "PauseState.h"
#include "OptionsState.h"
#include "HowToPlay.h"
#include "GameOver.h"
#include "../../Game.h"
#include "system/platform.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/sprite_renderer.h>
#include <graphics/font.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
StateMachine::StateMachine(gef::Platform& platform_) : platform(platform_)
{
	splashState = nullptr;
	mainMenuState = nullptr;
	level_1_intro_state = nullptr;
	level_1_state = nullptr;
	levelCompleteState = nullptr;
	pauseState = nullptr;
	optionsState = nullptr;
	howToPlayState = nullptr;
	gameOverState = nullptr;	
	currentState = nullptr;

	sprRend = nullptr;
	renderer3D = nullptr;
	font = nullptr;

	input_manager_ = nullptr;
	sc_ = nullptr;
	scim_ = nullptr;

	theWorld = nullptr;
	game = nullptr;

	audioManager = nullptr;
}

StateMachine::~StateMachine()
{
	delete splashState;
	splashState = nullptr;

	delete mainMenuState;
	mainMenuState = nullptr;

	delete level_1_intro_state;
	level_1_intro_state = nullptr;

	delete level_1_state;
	level_1_state = nullptr;

	delete levelCompleteState;
	levelCompleteState = nullptr;

	delete pauseState;
	pauseState = nullptr;

	delete optionsState;
	optionsState = nullptr;

	delete howToPlayState;
	howToPlayState = nullptr;

	delete gameOverState;
	gameOverState = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void StateMachine::onEnter()
{
	currentState->onEnter();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void StateMachine::onExit()
{
	currentState->onExit();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void StateMachine::handleInput(float dt)
{
	currentState->handleInput(dt);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool StateMachine::update(float dt)
{
	return currentState->update(dt);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void StateMachine::render()
{
	currentState->render();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void StateMachine::initGameWorld(Game* theGame, b2World* world, gef::AudioManager* audioMan)
{
	game = theGame;
	theWorld = world;
	audioManager = audioMan;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void StateMachine::initRend(gef::SpriteRenderer* spRend, gef::Renderer3D* rend3D, gef::Font* font_)
{
	sprRend = spRend;
	renderer3D = rend3D;
	font = font_;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void StateMachine::initInputControl(gef::InputManager* im, const gef::SonyController* sc, gef::SonyControllerInputManager* scim)
{
	input_manager_ = im;
	sc_ = sc;
	scim_ = scim;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void StateMachine::initStates()
{
	splashState = new SplashState(this);
	mainMenuState = new MenuState(this);
	level_1_intro_state = new Level_1_Intro(this);
	level_1_state = new Level_1_State(this);
	levelCompleteState = new LevelComplete(this);
	pauseState = new PauseState(this);
	optionsState = new OptionsState(this);
	howToPlayState = new HowToPlay(this);
	gameOverState = new GameOver(this);

	currentState = splashState;
	initStateMap();
	onEnter();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void StateMachine::initStateMap()
{
	states.insert(std::make_pair("Splash", splashState));
	states.insert(std::make_pair("MainMenu", mainMenuState));
	states.insert(std::make_pair("Level_1_Intro", level_1_intro_state));
	states.insert(std::make_pair("Level_1", level_1_state));
	states.insert(std::make_pair("LevelComplete", levelCompleteState));
	states.insert(std::make_pair("PauseMenu", pauseState));
	states.insert(std::make_pair("Options", optionsState));
	states.insert(std::make_pair("HowToPlay", howToPlayState));
	states.insert(std::make_pair("GameOver", gameOverState));
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// GETTERS / SETTERS
void StateMachine::setState(std::string newState)
{
	onExit();
	currentState = states[newState];
	onEnter();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

GameState* StateMachine::getCurrentState()
{
	return currentState;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Platform& StateMachine::getPlatform()
{
	return platform;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::SpriteRenderer* StateMachine::getSpriteRenderer()
{
	return sprRend;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Font* StateMachine::getFont()
{
	return font;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::InputManager* StateMachine::getInputManagaer()
{
	return input_manager_;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

const gef::SonyController* StateMachine::getSonyController()
{
	return sc_;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::SonyControllerInputManager* StateMachine::getSonyControllerIM()
{
	return scim_;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

b2World* StateMachine::getPhysicsWorld()
{
	return theWorld;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Renderer3D* StateMachine::get3DRenderer()
{
	return renderer3D;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

Game* StateMachine::getGame()
{
	return game;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::AudioManager* StateMachine::getAudioManager()
{
	return audioManager;
}

/////////////////////////////////////////////////////////////////////////////////////////////////