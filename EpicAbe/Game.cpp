#include "Game.h"
#include "src/char/Player.h"
#include "src/state/StateMachine.h"

#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <maths/math_utils.h>
#include <graphics/mesh.h>

Game::Game(gef::Platform& platform) :
	Application(platform),
	world_(NULL),
	quitGame(false)
{
}

void Game::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	// create the renderer for draw 3D geometry
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	initFont();
	initWorld();
	initInputManager();
	initStateMachine();
}

void Game::CleanUp()
{
	delete world_;
	world_ = nullptr;

	delete stateMachine;
	stateMachine = nullptr;
}

void Game::handleInput(float dt)
{
	stateMachine->handleInput(dt);
}

bool Game::Update(float frame_time)
{
	// THIS NEEDS TO BE THE FIRST THING DONE IN UPDATE!
	handleInput(frame_time);

	fps_ = 1.0f / frame_time;

	im->Update();

	quitGame = stateMachine->update(frame_time);

	// Update physics world
	float timeStep = 1.0f / 60.0f;
	world_->Step(timeStep, velocityIterations, positionIterations);

	if (quitGame)
	{
		return false;
	}

	return true;
}

void Game::Render()
{
	stateMachine->render();
}

void Game::initFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void Game::initInputManager()
{
	im = gef::InputManager::Create(platform_);
	scim = im->controller_input();
	controller = scim->GetController(0);
}

void Game::initWorld()
{
	// initialise the physics world
	velocityIterations = 6;
	positionIterations = 2;
	b2Vec2 gravity(0.0f, -9.81);
	world_ = new b2World(gravity);
}

void Game::initStateMachine()
{
	stateMachine = new StateMachine(platform_);
	stateMachine->init(platform_, sprite_renderer_, renderer_3d_, font_, world_);
	stateMachine->initInputControl(im, controller, scim);
	stateMachine->initStates();	
}