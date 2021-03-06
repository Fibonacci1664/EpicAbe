/*
 * This class controls all things related to the Level, including but not limited to:
 *		- Handling level debug information.
 *		- Creating a player.
 *		- Creating the environment.
 *		- Creating the UI.
 *		- Creating the level objects (Enemy & Collectables).
 *		- Checking player death.
 *		- All model loading.
 *		- All Animatoin loading.
 *		- Checking level compelte.
 *		- Handling input for accessing pause menu.
 *		- Updating and rendering all level objects.
 *		- Changing state based on various outcomes.
 *
 * Original @author D. Green.
 *
 * ? D. Green. 2021.
 */

/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StateMachine.h"
#include "Level_1_State.h"
#include "../char/Player.h"
#include "../char/Enemy.h"
#include "../env/Dunes.h"
#include "../env/Ground.h"
#include "../env/EnvPlatform.h"
#include "../collectable/Ruby.h"
#include "../hud/Heart.h"
#include "../env/LevelEndDoor.h"
#include "../env/Background.h"
#include "../env/Foreground.h"
#include "../env/LargePillar.h"
#include "../texture/load_texture.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/renderer_3d.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <input/sony_controller_input_manager.h>
#include <maths/math_utils.h>
#include <graphics/mesh.h>
#include <system/debug_log.h>
#include <graphics/colour.cpp>

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
Level_1_State::Level_1_State(StateMachine* sm)
{
	stateMachine = sm;
	stateGraphic = nullptr;
	fps = 0;
	stateTimer = 0;
	totalTimeElapsed = 0;
	isPaused = false;
	quitGame = false;
	isDebug = false;
	rubyStartPosOffsetX = 0;
	heartStartPosOffsetX = 0;
	startYPos = 7.0f;
	rotation = 0;

	initLevel();
}

Level_1_State::~Level_1_State()
{
	delete dunes;
	dunes = nullptr;

	delete background;
	background = nullptr;

	delete foreground;
	foreground = nullptr;

	delete largePillar;
	largePillar = nullptr;

	delete levelEndDoor;
	levelEndDoor = nullptr;

	delete ground;
	ground = nullptr;

	delete platform;
	platform = nullptr;

	delete ruby;
	ruby = nullptr;

	delete hudRuby;
	hudRuby = nullptr;

	delete hudHeart;
	hudHeart = nullptr;

	delete player;
	player = nullptr;

	for (int i = 0; i < enemies.size(); ++i)
	{
		delete enemies[i];
		enemies[i] = nullptr;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void Level_1_State::initLevel()
{
	initCamera();	
	initDunes();
	initBackground();
	initForeground();
	initLargePillar();
	initLevelEndDoor();
	initGround();
	initEnvPlatforms();
	initPlayer();
	initRubies();
	initHUDHearts();
	initEnemy();
	SetupLights();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::onEnter()
{
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::onExit()
{
	stateTimer = 0;
	isPaused = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::handleInput(float dt)
{
	if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_SELECT)
	{
		isPaused = true;		
	}

	player->handleInput(dt);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool Level_1_State::update(float dt)
{
	//stateTimer += dt;

	totalTimeElapsed += dt;
	rotation += dt;		// Rotate the hud icons

	if (!isPaused)
	{
		stateTimer += dt;
	}

	// CHECK VARIOUS STATE POTENTIALS
	if (isPaused)
	{
		stateMachine->setState("PauseMenu");
		return false;
	}

	if (player->getPlayerBody()->GetPosition().x > 98)
	{
		stateMachine->setState("LevelComplete");
		return false;
	}

	if (player->getLives() == 0)
	{
		stateMachine->setState("GameOver");
		return false;
	}
	// END CHECKING STATE POTENTIALS

	fps = 1.0f / dt;

	updateCamera();

	// Update physics world
	float timeStep = 1.0f / 60.0f;

	player->update(dt, stateMachine->getPhysicsWorld());
	ground->update(dt);
	updatePlatforms(dt);
	updateRubies(dt);
	updateHearts(dt);
	largePillar->update(dt);
	
	for (int i = 0; i < enemies.size(); ++i)
	{
		enemies[i]->update(dt);
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	player->render(stateMachine->get3DRenderer());

	for (int i = 0; i < enemies.size(); ++i)
	{
		enemies[i]->render(stateMachine->get3DRenderer());
	}

	dunes->render(stateMachine->get3DRenderer());
	background->render(stateMachine->get3DRenderer());
	foreground->render(stateMachine->get3DRenderer());
	largePillar->render(stateMachine->get3DRenderer());
	levelEndDoor->render(stateMachine->get3DRenderer());
	ground->render(stateMachine->get3DRenderer());

	renderPlatforms();
	renderRubies();

	// NOT BUILT YET!
	/*if (heart->getIsAlive())
	{
		heart->render(stateMachine->get3DRenderer());
	}*/
	
	hudRuby->render(stateMachine->get3DRenderer());
	hudHeart->render(stateMachine->get3DRenderer());
	stateMachine->getSpriteRenderer()->End();

	// start drawing sprites, but don't clear the frame buffer
	stateMachine->getSpriteRenderer()->Begin(false);
	DrawHUD();
	stateMachine->getSpriteRenderer()->End();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initCamera()
{
	// setup camera
	// projection
	float fov = gef::DegToRad(45.0f);
	float aspect_ratio = (float)stateMachine->getPlatform().width() / (float)stateMachine->getPlatform().height();
	projection_matrix = stateMachine->getPlatform().PerspectiveProjectionFov(fov, aspect_ratio, 0.1f, 100.0f);
	stateMachine->get3DRenderer()->set_projection_matrix(projection_matrix);

	// view
	gef::Vector4 camera_eye(0.0f, 5.0f, 15.0f);
	gef::Vector4 camera_lookat(0.0f, 0.0f, -5.0f);
	gef::Vector4 camera_up(0.0f, 1.0f, 0.0f);
	view_matrix.LookAt(camera_eye, camera_lookat, camera_up);
	stateMachine->get3DRenderer()->set_view_matrix(view_matrix);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initDunes()
{
	loadAsset("env/dunesCol.scn");

	gef::Mesh* dunesMesh = GetMeshFromSceneAssets(scene_assets_);

	dunes = new Dunes(gef::Vector4(15, 0, -2.3f), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 3.1415, 0));
	dunes->initDunes();

	if (scene_assets_)
	{
		dunes->set_mesh(dunesMesh);
	}
	else
	{
		gef::DebugOut("Dunes model failed to load\n");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initBackground()
{
	loadAsset("background.scn");

	gef::Mesh* backgroundMesh = GetMeshFromSceneAssets(scene_assets_);

	background = new Background(gef::Vector4(18.5f, 0, -1.9f), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 3.1415, 0));
	background->initBackground();

	if (scene_assets_)
	{
		background->set_mesh(backgroundMesh);
	}
	else
	{
		gef::DebugOut("Background model failed to load\n");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initForeground()
{
	loadAsset("foreground.scn");

	gef::Mesh* foregroundMesh = GetMeshFromSceneAssets(scene_assets_);

	foreground = new Foreground(gef::Vector4(21, -1, 2.9f), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 3.1415, 0));
	foreground->initForeground();

	if (scene_assets_)
	{
		foreground->set_mesh(foregroundMesh);
	}
	else
	{
		gef::DebugOut("Foreground model failed to load\n");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initLargePillar()
{
	loadAsset("largePillar.scn");

	gef::Mesh* largePillarMesh = GetMeshFromSceneAssets(scene_assets_);

	float xSize = largePillarMesh->aabb().max_vtx().x() - largePillarMesh->aabb().min_vtx().x();
	float ySize = largePillarMesh->aabb().max_vtx().y() - largePillarMesh->aabb().min_vtx().y();

	largePillar = new LargePillar(gef::Vector4(35, 0, 0.5), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 3.1415, 0));
	largePillar->initLargePillar(stateMachine->getPhysicsWorld(), xSize, ySize);

	if (scene_assets_)
	{
		largePillar->set_mesh(largePillarMesh);
	}
	else
	{
		gef::DebugOut("Large Pillar model failed to load\n");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initLevelEndDoor()
{
	loadAsset("levelEndDoor.scn");

	gef::Mesh* levelEndDoorMesh = GetMeshFromSceneAssets(scene_assets_);

	levelEndDoor = new LevelEndDoor(gef::Vector4(95, 0, 0), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 1.5707f, 0));
	levelEndDoor->initLevelEndDoor();

	if (scene_assets_)
	{
		levelEndDoor->set_mesh(levelEndDoorMesh);
	}
	else
	{
		gef::DebugOut("Level end door model failed to load\n");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initGround()
{
	loadAsset("ground.scn");

	gef::Mesh* groundMesh = GetMeshFromSceneAssets(scene_assets_);

	float xSize = groundMesh->aabb().max_vtx().x() - groundMesh->aabb().min_vtx().x();
	float ySize = groundMesh->aabb().max_vtx().y() - groundMesh->aabb().min_vtx().y();

	ground = new Ground(gef::Vector4(63.5, 0, 0), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 3.1415, 0));
	ground->initGround(stateMachine->getPhysicsWorld(), xSize, ySize);

	if (scene_assets_)
	{
		ground->set_mesh(groundMesh);
	}
	else
	{
		gef::DebugOut("Ground model failed to load\n");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initEnvPlatforms()
{
	loadAsset("smallPlatform.scn");

	gef::Mesh* smallPlatformMesh = GetMeshFromSceneAssets(scene_assets_);

	float xSize = smallPlatformMesh->aabb().max_vtx().x() - smallPlatformMesh->aabb().min_vtx().x();
	float ySize = smallPlatformMesh->aabb().max_vtx().y() - smallPlatformMesh->aabb().min_vtx().y();

	initLowEnvPlatforms(smallPlatformMesh, xSize, ySize);
	initMedEnvPlatforms(smallPlatformMesh, xSize, ySize);
	initHighEnvPlatforms(smallPlatformMesh, xSize, ySize);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initLowEnvPlatforms(gef::Mesh* smlPlatMesh, float sizeX, float sizeY)
{
	float lowPlatformIncrement = 0;

	// Init low platforms
	for (int i = 0; i < 5; ++i)
	{
		platform = new EnvPlatform(gef::Vector4(45 + lowPlatformIncrement, 1.15f, 0), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 3.1415, 0));
		platform->initEnvPlatform(stateMachine->getPhysicsWorld(), sizeX, sizeY);

		if (scene_assets_)
		{
			platform->set_mesh(smlPlatMesh);
		}
		else
		{
			gef::DebugOut("Small platform model failed to load\n");
		}

		lowPlatforms.push_back(platform);

		lowPlatformIncrement += 10.5f;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initMedEnvPlatforms(gef::Mesh* smlPlatMesh, float sizeX, float sizeY)
{
	float medPlatformIncrement = 0;

	// Init med platforms
	for (int i = 0; i < 5; ++i)
	{
		platform = new EnvPlatform(gef::Vector4(42 + medPlatformIncrement, 2.15f, 0), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 3.1415, 0));
		platform->initEnvPlatform(stateMachine->getPhysicsWorld(), sizeX, sizeY);

		if (scene_assets_)
		{
			platform->set_mesh(smlPlatMesh);
		}
		else
		{
			gef::DebugOut("Small platform model failed to load\n");
		}

		lowPlatforms.push_back(platform);

		medPlatformIncrement += 10.5f;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initHighEnvPlatforms(gef::Mesh* smlPlatMesh, float sizeX, float sizeY)
{
	float highPlatformIncrement = 0;

	// Init high platforms
	for (int i = 0; i < 5; ++i)
	{
		platform = new EnvPlatform(gef::Vector4(46 + highPlatformIncrement, 3, 0), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 3.1415, 0));
		platform->initEnvPlatform(stateMachine->getPhysicsWorld(), sizeX, sizeY);

		if (scene_assets_)
		{
			platform->set_mesh(smlPlatMesh);
		}
		else
		{
			gef::DebugOut("Small platform model failed to load\n");
		}

		lowPlatforms.push_back(platform);

		highPlatformIncrement += 10.5f;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initRubies()
{
	initCollectableRubies();
	initHUDRubies();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initCollectableRubies()
{
	loadAsset("ruby.scn");

	gef::Mesh* rubyMesh = GetMeshFromSceneAssets(scene_assets_);

	float xSize = rubyMesh->aabb().max_vtx().x() - rubyMesh->aabb().min_vtx().x();
	float ySize = rubyMesh->aabb().max_vtx().y() - rubyMesh->aabb().min_vtx().y();
	float rubyIncrement = 0;

	// Init rubies above high platforms
	for (int i = 0; i < 5; ++i)
	{
		ruby = new Ruby(gef::Vector4(46 + rubyIncrement, 3.5f, 0), gef::Vector4(2.0f, 2.0f, 2.0f), gef::Vector4(-1.5707f, 0, 0), false);
		ruby->initRuby(stateMachine->getPhysicsWorld(), xSize, ySize);

		if (scene_assets_)
		{
			ruby->set_mesh(rubyMesh);
		}
		else
		{
			gef::DebugOut("Ruby model failed to load\n");
		}

		rubies.push_back(ruby);

		rubyIncrement += 10.5f;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initHUDRubies()
{
	loadAsset("ruby.scn");

	gef::Mesh* HUDrubyMesh = GetMeshFromSceneAssets(scene_assets_);

	rubyStartPosOffsetX = 6.0f;

	hudRuby = new Ruby(gef::Vector4(player->getPosition()->x() + rubyStartPosOffsetX, startYPos, 0), gef::Vector4(4.0f, 4.0f, 4.0f), gef::Vector4(-1.5707f, 0, 0), true);

	if (scene_assets_)
	{
		hudRuby->set_mesh(HUDrubyMesh);
	}
	else
	{
		gef::DebugOut("HUD ruby model failed to load\n");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initHUDHearts()
{
	loadAsset("heart.scn");

	gef::Mesh* HUDheartMesh = GetMeshFromSceneAssets(scene_assets_);

	heartStartPosOffsetX = 8.2f;

	hudHeart = new Heart(gef::Vector4(player->getPosition()->x() - heartStartPosOffsetX, startYPos, 0), gef::Vector4(4.0f, 4.0f, 4.0f), gef::Vector4(-1.5707f, 0, 0), true);

	if (scene_assets_)
	{
		hudHeart->set_mesh(HUDheartMesh);
	}
	else
	{
		gef::DebugOut("HUD heart model failed to load\n");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initPlayer()
{
	player = new Player(gef::Vector4(40, 5, 0), gef::Vector4(0.005f, 0.005f, 0.005f), gef::Vector4(0, 0, 0));
	player->initInputManager(stateMachine->getPlatform(),
							stateMachine->getInputManagaer(),
							stateMachine->getSonyControllerIM(),
							stateMachine->getSonyController());
	player->initModelPlayer();
	player->initPhysicsBody(stateMachine->getPhysicsWorld());
	player->initAudio(stateMachine->getAudioManager(), stateMachine->getPlatform());
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::initEnemy()
{
	loadAsset("deathworm.scn");

	gef::Mesh* deathWormMesh = GetMeshFromSceneAssets(scene_assets_);

	float xSize = deathWormMesh->aabb().max_vtx().x() - deathWormMesh->aabb().min_vtx().x();
	float ySize = deathWormMesh->aabb().max_vtx().y() - deathWormMesh->aabb().min_vtx().y();

	int counter = 0;

	for (int i = 0; i < 5; ++i)
	{
		float randScale = (float)rand() / RAND_MAX;

		// Sets up a scale range for the enemy of 0.35 - 0.65
		if (randScale <= 0.1)
		{
			randScale += 0.25f;
		}
		else if (randScale >= 0.9)
		{
			randScale -= 0.25f;
		}

		// Sets up rand x location for the enemy of 40 - 70
		int randLocation = rand() % 40 + 70;

		Enemy* enemy = new Enemy(gef::Vector4(50 + counter, 5.0f, 0), gef::Vector4(randScale, randScale, randScale), gef::Vector4(0, 0, 4.71238f));
		enemy->initEnemy(stateMachine->getPhysicsWorld(), xSize, ySize);
		enemy->setStartLocation(50 + counter);

		if (scene_assets_)
		{
			enemy->set_mesh(deathWormMesh);
		}
		else
		{
			gef::DebugOut("Enemy model failed to load\n");
		}

		enemies.push_back(enemy);
		counter += 10;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::SetupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = stateMachine->get3DRenderer()->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	//// add a point light that is almost white, but with a blue tinge
	//// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	//default_point_light.set_colour(gef::Colour(0.988f, 0.611f, 0.322f, 1.0f)); Sunset colour
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::updateCamera()
{
	camPos = gef::Vector4(player->getPosition()->x(), 2.5f, player->getPosition()->z() + 12.0f);
	camLookAt = gef::Vector4(player->getPosition()->x(), 0, -1000000.0f);
	camUp = gef::Vector4(0.0f, 1.0f, 0.0f);

	view_matrix.LookAt(camPos, camLookAt, camUp);

	stateMachine->get3DRenderer()->set_projection_matrix(projection_matrix);
	stateMachine->get3DRenderer()->set_view_matrix(view_matrix);
}
/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::updateRubies(float dt)
{
	for (int i = 0; i < rubies.size(); ++i)
	{
		if (!rubies[i]->getIsBodyDestroyed())
		{
			rubies[i]->update(dt);
		}
	}

	hudRuby->setPosition(gef::Vector4(player->getPosition()->x() + rubyStartPosOffsetX, startYPos, 0));
	hudRuby->setRotation(gef::Vector4(-1.5707f, rotation, 0));
	hudRuby->update(dt);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::updateHearts(float dt)
{
	// NOT BUILT YET!
	/*if (!heart->getIsBodyDestroyed())
	{
		heart->update(dt);
	}*/

	hudHeart->setPosition(gef::Vector4(player->getPosition()->x() - heartStartPosOffsetX, startYPos, 0));
	hudHeart->setRotation(gef::Vector4(-1.5707f, rotation, 0));
	hudHeart->update(dt);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::updatePlatforms(float dt)
{
	for (int i = 0; i < lowPlatforms.size(); ++i)
	{
		lowPlatforms[i]->update(dt);
	}

	for (int i = 0; i < medPlatforms.size(); ++i)
	{
		medPlatforms[i]->update(dt);
	}

	for (int i = 0; i < highPlatforms.size(); ++i)
	{
		highPlatforms[i]->update(dt);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::renderPlatforms()
{
	for (int i = 0; i < lowPlatforms.size(); ++i)
	{
		lowPlatforms[i]->render(stateMachine->get3DRenderer());
	}

	for (int i = 0; i < medPlatforms.size(); ++i)
	{
		medPlatforms[i]->render(stateMachine->get3DRenderer());
	}

	for (int i = 0; i < highPlatforms.size(); ++i)
	{
		highPlatforms[i]->render(stateMachine->get3DRenderer());
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::renderRubies()
{
	for (int i = 0; i < rubies.size(); ++i)
	{
		if (rubies[i]->getIsAlive())
		{
			rubies[i]->render(stateMachine->get3DRenderer());
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::DrawHUD()
{
	// ACTUAL ON SCREEN GAME HUD STUFF
	stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(1250.0f, 30.0f, -0.9f), 1.0f, 0xff000000, gef::TJ_LEFT, "COLLECTED: %i", player->getRubiesCollected());
	stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(90.0f, 30.0f, -0.9f), 1.0f, 0xff000000, gef::TJ_LEFT, "LIVES: %i", player->getLives());

	// Switch isDebug ON/OFF in the CONSTRUCTOR.
	if (isDebug)
	{
		if (stateMachine->getFont())
		{
			// IF SETTING COLOUR THEN THE ALPHA CHANNEL IS FIRST.
			// display frame rate
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 60.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "FPS: %.1f", fps);

			// Controller output.
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 105.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "XButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_CROSS ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 120.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "SquareButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_SQUARE ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 135.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "CircleButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_CIRCLE ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 150.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "TriangleButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_TRIANGLE ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 165.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "L1ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_L1 ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 180.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "L2ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_L2 ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 195.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "L3ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_L3 ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 210.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "R1ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_R1 ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 225.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "R2ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_R2 ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 240.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "R3ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_R3 ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 255.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "upDButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_UP ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 270.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "downDButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_DOWN ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 285.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "leftDButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_LEFT ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 300.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "rightDButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_RIGHT ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 315.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "shareButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_START ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 330.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "optionsButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_SELECT ? "Yes" : "No");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 345.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "leftStickX: %.1f", player->getSonyController()->left_stick_x_axis());
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 360.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "leftStickY: %.1f", player->getSonyController()->left_stick_y_axis());
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 375.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "rightStickX: %.1f", player->getSonyController()->right_stick_x_axis());
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 390.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "rightStickY: %.1f", player->getSonyController()->right_stick_y_axis());
			//stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 405.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "rightStickAngle: %.1f", cam->getRightStickAngle());
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 420.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "leftStickAngle: %.1f", player->getLeftStickAngle());
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 435.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "Player Y-Pos: %.1f", player->getPlayerYPos());
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 450.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "Player grounded: %s", player->isOnGround() ? "True" : "False");
			stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 465.0f, -0.9f), 0.5f, 0xff000000, gef::TJ_LEFT, "Player Health: %i", player->getLives());
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Level_1_State::loadAsset(const char* assetFilePath)
{
	const char* asset = assetFilePath;
	scene_assets_ = LoadSceneAssets(stateMachine->getPlatform(), assetFilePath);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Scene* Level_1_State::LoadSceneAssets(gef::Platform& platform, const char* filename)
{
	gef::Scene* scene = new gef::Scene();

	if (scene->ReadSceneFromFile(platform, filename))
	{
		// if scene file loads successful
		// create material and mesh resources from the scene data
		scene->CreateMaterials(platform);
		scene->CreateMeshes(platform);
	}
	else
	{
		delete scene;
		scene = NULL;
	}

	return scene;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Mesh* Level_1_State::GetMeshFromSceneAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	// if the scene data contains at least one mesh
	// return the first mesh
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();

	return mesh;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Texture* Level_1_State::initStateGraphic(gef::Platform& platform_)
{
	stateGraphic = CreateTextureFromPNG("level_1.png", platform_);

	return stateGraphic;
}

/////////////////////////////////////////////////////////////////////////////////////////////////