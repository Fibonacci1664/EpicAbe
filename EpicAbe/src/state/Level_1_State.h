#pragma once
#include "GameState.h"

#include <system/application.h>
#include <maths/vector2.h>
#include <box2d/box2d.h>
#include <graphics/scene.h>
#include <maths/matrix44.h>

const int WIDTH = 2;
const int DEPTH = 1;

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Scene;
	class Platform;
	class Mesh;
	class InputManager;
	class SonyController;
	class SonyControllerInputManager;
}

class StateMachine;
class Environment;
class Railing;
class DirtGround;
class Player;
class Enemy;

class Level_1_State : public GameState
{
public:
	Level_1_State(StateMachine* sm);
	~Level_1_State() override;

	void onEnter() override;
	void onExit() override;
	void handleInput(float dt) override;
	bool update(float dt) override;
	void render() override;

protected:

private:
	void initCamera();
	void initPlayer();
	//void initEnemy();
	void initGround();
	void initDunes();
	void initBackdropScenery();
	void updateCamera();
	void DrawHUD();
	void SetupLights();

	void loadAsset(const char* assetFilePath);
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	void CleanUpFont();

	// CAMERA STUFF
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;

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

	// LEVEL MODEL LOADING STUFF
	gef::Scene* scene_assets_;

	// STATE MACHINE STUFF;
	StateMachine* stateMachine;

	float fps;
	float stateTimer;
	float totalTimeElapsed;
	bool isPaused;
	bool quitGame;
};