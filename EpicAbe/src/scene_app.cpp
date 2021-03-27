#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <maths/math_utils.h>
#include <graphics/mesh.h>

#include "Player.h"

SceneApp::SceneApp(gef::Platform& platform):
	Application(platform)
	,sprite_renderer_(NULL)
	,renderer_3d_(NULL)
	,font_(NULL)
	,world_(NULL)
	,scene_assets_(NULL)
{
}

void SceneApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	// create the renderer for draw 3D geometry
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	InitFont();
	SetupLights();
	initWorld();
	initCamera();
	initInputManager();
	initPlayer();
	//initEnemy();	
	initGround();
	initRailing();
	initDirtGround();
}

void SceneApp::CleanUp()
{
	// clean up scene assets
	delete scene_assets_;
	scene_assets_ = NULL;

	CleanUpFont();


	delete renderer_3d_;
	renderer_3d_ = NULL;

	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete world_;
	world_ = nullptr;

	delete scene_assets_;
	scene_assets_ = nullptr;

	/*delete env;
	env = nullptr;*/

	for (int i = 0; i < grounds.size(); ++i)
	{
		delete grounds[i];
		grounds[i] = nullptr;
	}

	//for (int i = 0; i < railings.size(); ++i)
	//{
	//	delete railings[i];
	//	railings[i] = nullptr;
	//}

	/*for (int x = 0; x < WIDTH; ++x)
	{
		for (int z = 0; z < DEPTH; ++z)
		{
			delete dirtGrounds[x][z];
			dirtGrounds[x][z] = nullptr;
		}
	}*/

	delete dirtGround;
	dirtGround = nullptr;

	delete player;
	player = nullptr;
}

bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;
	
	im->Update();

	updateCamera();

	// Update physics world
	float timeStep = 1.0f / 60.0f;

	world_->Step(timeStep, velocityIterations, positionIterations);
	quit = player->update(frame_time, world_);
	/*enemy->update(frame_time);
	enemy->followPlayer(player, frame_time);*/

	for (int i = 0; i < grounds.size(); ++i)
	{
		grounds[i]->update(frame_time);
	}

	// Update railings transform. Could this be done in the Railings Constructor? YES!
	/*for (int i = 0; i < railings.size(); ++i)
	{
		railings[i]->update(frame_time);
	}*/

	//env->update(frame_time);
	

	if (quit)
	{
		return false;
	}

	return true;
}

void SceneApp::Render()
{
	// draw 3d geometry
	renderer_3d_->Begin();

	player->render(renderer_3d_);
	//enemy->render(renderer_3d_);

	for (int i = 0; i < grounds.size(); ++i)
	{
		grounds[i]->render(renderer_3d_);
	}

	dirtGround->render(renderer_3d_);

	for (int i = 0; i < railings.size(); ++i)
	{
		railings[i]->render(renderer_3d_);
	}

	/*for (int x = 0; x < WIDTH; ++x)
	{
		for (int z = 0; z < DEPTH; ++z)
		{
			dirtGrounds[x][z]->render(renderer_3d_);
		}
	}*/

	//env->render(renderer_3d_);	

	renderer_3d_->End();

	// start drawing sprites, but don't clear the frame buffer
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}
void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void SceneApp::initInputManager()
{
	im = gef::InputManager::Create(platform_);
	scim = im->controller_input();
	controller = scim->GetController(0);
}

void SceneApp::initCamera()
{
	// setup camera
	// projection
	float fov = gef::DegToRad(45.0f);
	float aspect_ratio = (float)platform_.width() / (float)platform_.height();
	projection_matrix = platform_.PerspectiveProjectionFov(fov, aspect_ratio, 0.1f, 100.0f);
	renderer_3d_->set_projection_matrix(projection_matrix);
		
	// view
	gef::Vector4 camera_eye(0.0f, 5.0f, 5.0f);
	gef::Vector4 camera_lookat(0.0f, 0.0f, -5.0f);
	gef::Vector4 camera_up(0.0f, 1.0f, 0.0f);
	view_matrix.LookAt(camera_eye, camera_lookat, camera_up);
	renderer_3d_->set_view_matrix(view_matrix);
}

void SceneApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void SceneApp::DrawHUD()
{
	if(font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 60.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);

		// Controller output.
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 105.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "XButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_CROSS ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 120.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "SquareButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_SQUARE ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 135.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "CircleButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_CIRCLE ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 150.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "TriangleButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_TRIANGLE ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 165.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "L1ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_L1 ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 180.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "L2ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_L2 ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 195.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "L3ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_L3 ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 210.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "R1ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_R1 ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 225.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "R2ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_R2 ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 240.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "R3ButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_R3 ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 255.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "upDButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_UP ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 270.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "downDButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_DOWN ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 285.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "leftDButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_LEFT ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 300.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "rightDButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_RIGHT ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 315.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "shareButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_START ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 330.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "optionsButtonPressed: %s", player->getSonyController()->buttons_down() & gef_SONY_CTRL_SELECT ? "Yes" : "No");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 345.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "leftStickX: %.1f", player->getSonyController()->left_stick_x_axis());
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 360.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "leftStickY: %.1f", player->getSonyController()->left_stick_y_axis());
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 375.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "rightStickX: %.1f", player->getSonyController()->right_stick_x_axis());
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 390.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "rightStickY: %.1f", player->getSonyController()->right_stick_y_axis());
		//font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 405.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "rightStickAngle: %.1f", cam->getRightStickAngle());
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 420.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "leftStickAngle: %.1f", player->getLeftStickAngle());
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 435.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "Player Y-Pos: %.1f", player->getPlayerYPos());
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 450.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "Player grounded: %s", player->isOnGround() ? "True" : "False");
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 465.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "Player Health: %i", player->getPlayerHealth());
	}
}

void SceneApp::SetupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	// add a point light that is almost white, but with a blue tinge
	// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void SceneApp::initWorld()
{
	// initialise the physics world
	velocityIterations = 6;
	positionIterations = 2;
	b2Vec2 gravity(0.0f, -9.81);
	world_ = new b2World(gravity);
}

void SceneApp::initPlayer()
{
	//gef::Vector4(0, 3.1415 * 0.5f, 0))

	player = new Player(gef::Vector4(17, 5, 0), gef::Vector4(0.005f, 0.005f, 0.005f), gef::Vector4(0, 3.1415f, 0));
	player->initInputManager(platform_, im, scim, controller);
	player->initModelPlayer();
	player->initPhysicsBody(world_);
	//player->initPlayer(primitive_builder_, world_);	
}

//void SceneApp::initEnemy()
//{
//	enemy = new Enemy(gef::Vector4(-4.0f, 4.0f, 0), gef::Vector4(0.4f, 0.4f, 0.4f), gef::Vector4(0, 0, 0));
//	enemy->initEnemy(primitive_builder_, world_);
//}

void SceneApp::initGround()
{
	loadAsset("env/desert_ground.scn");

	gef::Mesh* floorMesh = GetMeshFromSceneAssets(scene_assets_);

	float xSize = floorMesh->aabb().max_vtx().x() - floorMesh->aabb().min_vtx().x();
	float ySize = floorMesh->aabb().max_vtx().y() - floorMesh->aabb().min_vtx().y();

	for (int i = 0; i < 10; ++i)
	{
		//env = new Environment(gef::Vector4(-5 + i, 0, 0), gef::Vector4(xSize, ySize * 0.05f, 1.0f), gef::Vector4(0, 3.1415 0));
		env = new Environment(gef::Vector4(5 + (i * 11), 0, 0), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 3.1415, 0));
		env->initGround(world_, xSize, ySize);

		if (scene_assets_)
		{
			env->set_mesh(floorMesh);
		}
		else
		{
			gef::DebugOut("Ground model failed to load\n");
		}

		grounds.push_back(env);
	}

	// Single floor env object.
	/*env = new Environment(gef::Vector4(0, 0, 0), gef::Vector4(1.0f, 0.01f, 1.0f), gef::Vector4(0, 0, 0));
	env->initGround(world_);
	loadAsset(env, "stone_floor.scn");*/
}

void SceneApp::initDirtGround()
{
	loadAsset("env/dunesCol.scn");

	gef::Mesh* dirtGroundMesh = GetMeshFromSceneAssets(scene_assets_);

	float xSize = dirtGroundMesh->aabb().max_vtx().x() - dirtGroundMesh->aabb().min_vtx().x();
	float zSize = dirtGroundMesh->aabb().max_vtx().z() - dirtGroundMesh->aabb().min_vtx().z();

	dirtGround = new DirtGround(gef::Vector4(5, 0, -2.3f), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 3.1415, 0));
	dirtGround->initDirt();

	if (scene_assets_)
	{
		dirtGround->set_mesh(dirtGroundMesh);
	}
	else
	{
		gef::DebugOut("Dirt ground model failed to load\n");
	}

	/*for (int x = 0; x < WIDTH; ++x)
	{
		for (int z = 0; z < DEPTH; ++z)
		{
			dirtGround = new DirtGround(gef::Vector4(5.0f + (x * 2.0f), 0, -2.0f - (z * 2.0f)), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 3.1415, 0));
			dirtGround->initDirt();

			if (scene_assets_)
			{
				dirtGround->set_mesh(dirtGroundMesh);
			}
			else
			{
				gef::DebugOut("Dirt ground model failed to load\n");
			}

			dirtGrounds[x][z] = dirtGround;
		}
	}*/
}

void SceneApp::initRailing()
{
	loadAsset("env/desert.scn");

	gef::Mesh* railingMesh = GetMeshFromSceneAssets(scene_assets_);

	float xSize = railingMesh->aabb().max_vtx().x() - railingMesh->aabb().min_vtx().x();
	float zSize = railingMesh->aabb().max_vtx().z() - railingMesh->aabb().min_vtx().z();

	for (int i = 0; i < 15; ++i)
	{
		railing = new Railing(gef::Vector4(5 + (i * 7), 0, -2.0f), gef::Vector4(1.0f, 1.0f, 1.0f), gef::Vector4(0, 3.1415, 0));
		railing->initRailing();

		if (scene_assets_)
		{
			railing->set_mesh(railingMesh);
		}
		else
		{
			gef::DebugOut("Railing model failed to load\n");
		}

		railings.push_back(railing);
	}
}

void SceneApp::updateCamera()
{
	/*int offSet = 3.0f;
	gef::Vector4 newCamPos;
	
	if (player->getIsFacingLeft())
	{
		offSet = 6.0f;
		gef::Vector4 newCamPos = gef::Vector4(player->getPosition()->x() + offSet, 2.5f, player->getPosition()->z() + 7.0f);
	}
	else
	{
		offSet = 3.0f;
		gef::Vector4 newCamPos = gef::Vector4(player->getPosition()->x() + offSet, 2.5f, player->getPosition()->z() + 7.0f);
	}

	camPos.Lerp(camPos, newCamPos, 1.0f);
	camPos = newCamPos;*/

	gef::Vector4 camPos = gef::Vector4(player->getPosition()->x() + 3.0f, 2.5f, player->getPosition()->z() + 7.0f);
	gef::Vector4 camLookAt = gef::Vector4(player->getPosition()->x(), 0, -1000000.0f);
	gef::Vector4 camUp = gef::Vector4(0.0f, 1.0f, 0.0f);

	view_matrix.LookAt(camPos, camLookAt, camUp);

	renderer_3d_->set_projection_matrix(projection_matrix);
	renderer_3d_->set_view_matrix(view_matrix);
}

void SceneApp::loadAsset(const char* assetFilePath)
{
	const char* asset = assetFilePath;
	scene_assets_ = LoadSceneAssets(platform_, assetFilePath);
}

gef::Scene* SceneApp::LoadSceneAssets(gef::Platform& platform, const char* filename)
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

gef::Mesh* SceneApp::GetMeshFromSceneAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	// if the scene data contains at least one mesh
	// return the first mesh
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();

	return mesh;
}
