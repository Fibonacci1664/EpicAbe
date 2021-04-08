#pragma once
#include "GameState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <system/application.h>
#include <maths/vector2.h>
#include <box2d/box2d.h>
#include <graphics/scene.h>
#include <maths/matrix44.h>
#include <../gef_abertay/graphics/point_light.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

// FORWARD DECS
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
class Ground;
class LevelEndDoor;
class Background;
class Foreground;
class LargePillar;
class Dunes;
class Player;
class Enemy;
class EnvPlatform;
class Ruby;
class Heart;

/////////////////////////////////////////////////////////////////////////////////////////////////

// GLOBALS
const int WIDTH = 2;
const int DEPTH = 1;

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
	void initLevel();
	void initCamera();
	void initPlayer();
	void initEnemy();
	void initGround();
	void initEnvPlatforms();
	void initRubies();
	void initCollectableRubies();
	void initHUDRubies();
	void initHUDHearts();
	void initDunes();
	void initBackground();
	void initForeground();
	void initLargePillar();
	void initLevelEndDoor();
	void updateCamera();
	void updateRubies(float dt);
	void updateHearts(float dt);
	void DrawHUD();
	void SetupLights();

	void loadAsset(const char* assetFilePath);
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	gef::Texture* initStateGraphic(gef::Platform& platform_);

	// CAMERA STUFF
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;

	gef::Vector4 camPos;
	gef::Vector4 camLookAt;
	gef::Vector4 camUp;

	// DUNES STUFF
	Dunes* dunes;

	// BACKGROUND STUFF
	Background* background;

	// FOREGROUND STUFF
	Foreground* foreground;

	// LARGE PILLAR SCENE BLOCKER
	LargePillar* largePillar;

	// LEVEL END DOOR STUFF
	LevelEndDoor* levelEndDoor;

	// GROUND STUFF
	Ground* ground;

	// ENV PLATFORMS STUFF
	EnvPlatform* smallPlatform;

	// COLLECTABLE RUBY STUFF
	Ruby* ruby;

	// HUD STUFF GENERAL
	float startYPos;
	float rotation;

	// HUD RUBY STUFF
	Ruby* hudRuby;
	float rubyStartPosOffsetX;
	
	// HUD HEART STUFF
	Heart* hudHeart;
	float heartStartPosOffsetX;

	// PLAYER STUFF
	Player* player;
	gef::PointLight playerLight;

	// ENEMY STUFF
	std::vector<Enemy*> enemies;

	// LEVEL MODEL LOADING STUFF
	gef::Scene* scene_assets_;

	// STATE MACHINE STUFF
	StateMachine* stateMachine;

	float fps;
	float stateTimer;
	float totalTimeElapsed;
	bool isPaused;
	bool quitGame;
	bool isDebug;
};

/////////////////////////////////////////////////////////////////////////////////////////////////