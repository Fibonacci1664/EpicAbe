#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
//#include <graphics/mesh_instance.h>
#include <box2d/box2d.h>
#include <graphics/scene.h>

#include "Environment.h"
#include "Railing.h"
#include "DirtGround.h"

const int WIDTH = 2;
const int DEPTH = 1;

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Scene;
	class Platform;
	class Mesh;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class InputManager;
	class SonyController;
	class SonyControllerInputManager;
}

	class b2World;
	class Environment;
	class Railing;
	class DirtGround;
	class Player;
	class Enemy;

class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:
	void InitFont();
	void initInputManager();
	void initCamera();
	void initWorld();
	void initPlayer();
	//void initEnemy();
	void initGround();
	void initDirtGround();
	void initRailing();
	void updateCamera();
	void CleanUpFont();
	void DrawHUD();
	void SetupLights();
	void loadAsset(const char* assetFilePath);
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);
 
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::Renderer3D* renderer_3d_;

	// INPUT MANAGER STUFF
	gef::InputManager* im;
	const gef::SonyController* controller;
	gef::SonyControllerInputManager* scim;

	// CAMERA STUFF
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;

	// PHYSICS WORLD STUFF
	b2World* world_;
	int32 velocityIterations;
	int32 positionIterations;

	// ENVIRONMENT STUFF
	std::vector<Environment*> grounds;
	Environment* env;

	// RAILING STUFF
	std::vector<Railing*> railings;
	Railing* railing;

	// DIRTGROUND STUFF
	//DirtGround* dirtGrounds[WIDTH][DEPTH];
	DirtGround* dirtGround;

	// PLAYER STUFF
	Player* player;

	// ENEMY STUFF
	Enemy* enemy;

	gef::Scene* scene_assets_;

	float fps_;
	bool quit = false;
};

#endif // _SCENE_APP_H
