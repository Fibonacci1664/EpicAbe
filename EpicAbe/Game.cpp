/*
 * This class controls
 *		- Initialising:
 *			- Fonts
 *			- The Physics World
 *			- Input
 *			- Audio
 *			- The Games State Machine
 *
 * Original @author Abertay University
 *
 * Updated by @author D. Green.
 *
 * © D. Green. 2021.
 */

/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Game.h"
#include "src/char/Player.h"
#include "src/state/StateMachine.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <maths/math_utils.h>
#include <graphics/mesh.h>
#include <audio/audio_manager.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR
Game::Game(gef::Platform& platform) :
	Application(platform),
	font_(nullptr),
	world_(NULL),
	controller(nullptr),
	im(nullptr),
	scim(nullptr),
	fps_(0),
	positionIterations(0),
	velocityIterations(0),
	renderer_3d_(nullptr),
	sprite_renderer_(nullptr),
	stateMachine(nullptr),
	quitGame(false)
{

	audioManager = nullptr;
	gameMusic = 0;
	//sampleSound = 0;
	gameMusicVolinfo = nullptr;
	musicVolume = 1.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void Game::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	// create the renderer for draw 3D geometry
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	initFont();
	initWorld();
	initInputManager();
	initAudio();
	initStateMachine();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Game::CleanUp()
{
	delete font_;
	font_ = nullptr;

	delete world_;
	world_ = nullptr;

	delete stateMachine;
	stateMachine = nullptr;

	delete gameMusicVolinfo;
	gameMusicVolinfo = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Game::handleInput(float dt)
{
	// This sets of the chain of handling ALL input throughout the states before ANY updating is done.
	stateMachine->handleInput(dt);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool Game::Update(float frame_time)
{
	// THIS NEEDS TO BE THE FIRST THING DONE IN UPDATE!
	handleInput(frame_time);

	fps_ = 1.0f / frame_time;

	// Once all the handling of input is done, update stuff.
	im->Update();
	quitGame = stateMachine->update(frame_time);

	// Update physics world
	float timeStep = 1.0f / 60.0f;
	world_->Step(timeStep, velocityIterations, positionIterations);

	// If quitGame ever returns true this then returns false back to the main game while loop
	// and thus exits the loop and performs the final clean up.
	if (quitGame)
	{
		return false;
	}

	// Ohterwise keep running the main game loop.
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Game::Render()
{
	stateMachine->render();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Game::initFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

/////////////////////////////////////////////////////////////////////////////////////////////////


void Game::initWorld()
{
	// initialise the physics world
	velocityIterations = 6;
	positionIterations = 2;
	b2Vec2 gravity(0.0f, -9.81f);
	world_ = new b2World(gravity);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Game::initInputManager()
{
	im = gef::InputManager::Create(platform_);
	scim = im->controller_input();
	controller = scim->GetController(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Game::initStateMachine()
{
	stateMachine = new StateMachine(platform_);
	stateMachine->initGameWorld(this, world_, audioManager);
	stateMachine->initRend(sprite_renderer_, renderer_3d_, font_);
	stateMachine->initInputControl(im, controller, scim);
	stateMachine->initStates();	
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Game::initAudio()
{
	// initialise audio manager
	audioManager = gef::AudioManager::Create();
	//sampleSound = audioManager->LoadSample("../media/box_collected.wav", platfrom_);
	gameMusic = audioManager->LoadMusic("desertLoop.wav", platform_);

	gameMusicVolinfo = new gef::VolumeInfo();
	gameMusicVolinfo->volume = 50;
	audioManager->SetMusicVolumeInfo(*gameMusicVolinfo);

	audioManager->PlayMusic();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// GETTERS/ SETTERS
float Game::getGameVolume()
{
	return gameMusicVolinfo->volume;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Game::setGameVolume(float newVol)
{
	gameMusicVolinfo->volume = newVol;
	audioManager->SetMusicVolumeInfo(*gameMusicVolinfo);
}

/////////////////////////////////////////////////////////////////////////////////////////////////