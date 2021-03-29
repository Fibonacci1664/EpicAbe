#include "Level_1_State.h"
#include "StateMachine.h"
#include "../char/Player.h"
#include "../env/Dunes.h"
#include "../env/Ground.h"
#include "../env/Background.h"

#include <graphics/renderer_3d.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <input/sony_controller_input_manager.h>
#include <maths/math_utils.h>
#include <graphics/mesh.h>
#include <system/debug_log.h>

Level_1_State::Level_1_State(StateMachine* sm)
{
	stateMachine = sm;
	fps = 0;
	stateTimer = 0;
	totalTimeElapsed = 0;
	isPaused = false;
	quitGame = false;

	initCamera();
	SetupLights();	
	initDunes();
	initBackdropScenery();
	initGround();
	initPlayer();
}

Level_1_State::~Level_1_State()
{
	
}

void Level_1_State::onEnter()
{
	//updateCamera();
}

void Level_1_State::onExit()
{
	stateTimer = 0;
	isPaused = false;
}

void Level_1_State::handleInput(float dt)
{
	if (stateMachine->getSonyController()->buttons_released() & gef_SONY_CTRL_SELECT)
	{
		isPaused = true;		
	}
}

bool Level_1_State::update(float dt)
{
	// THIS NEEDS TO BE THE FIRST THING DONE IN UPDATE!
	handleInput(dt);

	totalTimeElapsed += dt;

	if (!isPaused)
	{
		stateTimer += dt;
	}

	if (isPaused)
	{
		stateMachine->setState("PauseMenu");
	}

	fps = 1.0f / dt;

	updateCamera();

	// Update physics world
	float timeStep = 1.0f / 60.0f;

	player->update(dt, stateMachine->getPhysicsWorld());
	/*enemy->update(frame_time);
	enemy->followPlayer(player, frame_time);*/

	return false;
}

void Level_1_State::render()
{
	stateMachine->getSpriteRenderer()->Begin();

	player->render(stateMachine->get3DRenderer());
	dunes->render(stateMachine->get3DRenderer());
	background->render(stateMachine->get3DRenderer());
	ground->render(stateMachine->get3DRenderer());
	stateMachine->getSpriteRenderer()->End();

	// start drawing sprites, but don't clear the frame buffer
	stateMachine->getSpriteRenderer()->Begin(false);
	DrawHUD();
	stateMachine->getSpriteRenderer()->End();
}

void Level_1_State::initCamera()
{
	// setup camera
	// projection
	float fov = gef::DegToRad(45.0f);
	float aspect_ratio = (float)stateMachine->getPlatform().width() / (float)stateMachine->getPlatform().height();
	projection_matrix = stateMachine->getPlatform().PerspectiveProjectionFov(fov, aspect_ratio, 0.1f, 100.0f);
	stateMachine->get3DRenderer()->set_projection_matrix(projection_matrix);

	// view
	gef::Vector4 camera_eye(0.0f, 5.0f, 5.0f);
	gef::Vector4 camera_lookat(0.0f, 0.0f, -5.0f);
	gef::Vector4 camera_up(0.0f, 1.0f, 0.0f);
	view_matrix.LookAt(camera_eye, camera_lookat, camera_up);
	stateMachine->get3DRenderer()->set_view_matrix(view_matrix);
}

void Level_1_State::initPlayer()
{
	player = new Player(gef::Vector4(17, 5, 0), gef::Vector4(0.005f, 0.005f, 0.005f), gef::Vector4(0, 3.1415f, 0));
	player->initInputManager(stateMachine->getPlatform(),
							stateMachine->getInputManagaer(),
							stateMachine->getSonyControllerIM(),
							stateMachine->getSonyController());
	player->initModelPlayer();
	player->initPhysicsBody(stateMachine->getPhysicsWorld());
}

void Level_1_State::initGround()
{
	loadAsset("env/desert_ground.scn");

	gef::Mesh* groundMesh = GetMeshFromSceneAssets(scene_assets_);

	float xSize = groundMesh->aabb().max_vtx().x() - groundMesh->aabb().min_vtx().x();
	float ySize = groundMesh->aabb().max_vtx().y() - groundMesh->aabb().min_vtx().y();

	//env = new Environment(gef::Vector4(-5 + i, 0, 0), gef::Vector4(xSize, ySize * 0.05f, 1.0f), gef::Vector4(0, 3.1415 0));
	ground = new Ground(gef::Vector4(15, 0, 0), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 3.1415, 0));
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

void Level_1_State::initBackdropScenery()
{
	loadAsset("env/desert.scn");

	gef::Mesh* backgroundMesh = GetMeshFromSceneAssets(scene_assets_);

	background = new Background(gef::Vector4(15, 0, -2.0f), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 3.1415, 0));
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

void Level_1_State::updateCamera()
{
	gef::Vector4 camPos = gef::Vector4(player->getPosition()->x() + 3.0f, 2.5f, player->getPosition()->z() + 7.0f);
	gef::Vector4 camLookAt = gef::Vector4(player->getPosition()->x(), 0, -1000000.0f);
	gef::Vector4 camUp = gef::Vector4(0.0f, 1.0f, 0.0f);

	view_matrix.LookAt(camPos, camLookAt, camUp);

	stateMachine->get3DRenderer()->set_projection_matrix(projection_matrix);
	stateMachine->get3DRenderer()->set_view_matrix(view_matrix);
}

void Level_1_State::DrawHUD()
{
	if (stateMachine->getFont())
	{
		// display frame rate
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 60.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps);

		// Controller output.
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 105.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "XButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_CROSS ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 120.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "SquareButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_SQUARE ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 135.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "CircleButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_CIRCLE ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 150.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "TriangleButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_TRIANGLE ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 165.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "L1ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_L1 ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 180.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "L2ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_L2 ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 195.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "L3ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_L3 ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 210.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "R1ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_R1 ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 225.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "R2ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_R2 ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 240.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "R3ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_R3 ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 255.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "upDButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_UP ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 270.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "downDButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_DOWN ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 285.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "leftDButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_LEFT ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 300.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "rightDButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_RIGHT ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 315.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "shareButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_START ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 330.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "optionsButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_SELECT ? "Yes" : "No");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 345.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "leftStickX: %.1f", player->getSonyController()->left_stick_x_axis());
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 360.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "leftStickY: %.1f", player->getSonyController()->left_stick_y_axis());
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 375.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "rightStickX: %.1f", player->getSonyController()->right_stick_x_axis());
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 390.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "rightStickY: %.1f", player->getSonyController()->right_stick_y_axis());
		//stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 405.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "rightStickAngle: %.1f", cam->getRightStickAngle());
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 420.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "leftStickAngle: %.1f", player->getLeftStickAngle());
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 435.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "Player Y-Pos: %.1f", player->getPlayerYPos());
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 450.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "Player grounded: %s", player->isOnGround() ? "True" : "False");
		stateMachine->getFont()->RenderText(stateMachine->getSpriteRenderer(), gef::Vector4(10.0f, 465.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "Player Health: %i", player->getPlayerHealth());
	}
}

void Level_1_State::SetupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = stateMachine->get3DRenderer()->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	// add a point light that is almost white, but with a blue tinge
	// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void Level_1_State::loadAsset(const char* assetFilePath)
{
	const char* asset = assetFilePath;
	scene_assets_ = LoadSceneAssets(stateMachine->getPlatform(), assetFilePath);
}

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

gef::Mesh* Level_1_State::GetMeshFromSceneAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	// if the scene data contains at least one mesh
	// return the first mesh
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();

	return mesh;
}