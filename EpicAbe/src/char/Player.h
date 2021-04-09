#pragma once
#include "../object/game_object.h"
#include "../anim/motion_clip_player.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/renderer_3d.h>
#include <graphics/mesh_instance.h>
#include <input/sony_controller_input_manager.h>
#include <input/input_manager.h>
#include <cmath>
#include <maths/math_utils.h>
#include <maths/transform.h>
#include <box2d/box2d.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

// FORWARD DECS
namespace gef
{
	class Scene;
	class Mesh;
	class Skeleton;
	class SkinnedMeshInstance;
	class Animation;
	class Renderer3D;
	class InputManager;
	class SonyControllerInputManager;
	class SonyController;
	class AudioManager;
	class VolumeInfo;
	class Platform;
}

class b2World;

/////////////////////////////////////////////////////////////////////////////////////////////////

class Player : public GameObject
{
public:
	Player(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation);
	~Player();

	void handleInput(float dt);
	void checkCollisions(float dt, b2World* world_);
	bool update(float dt, b2World* world_);
	void render(gef::Renderer3D* rend3D);

	void initInputManager(gef::Platform& platform,
		gef::InputManager* im,
		gef::SonyControllerInputManager* scim,
		const gef::SonyController* controller);
	void initModelPlayer();
	void initPhysicsBody(b2World* world);
	void initAudio(gef::AudioManager* audioMan, gef::Platform& thePlatform);

	// GETTERS / SETTERS
	void setButtonState();
	const gef::SonyController* getSonyController();
	gef::Vector4* getPosition();
	void setRotation(gef::Vector4 newRot);
	float getLeftStickAngle();
	float getPlayerYPos();
	bool getHasCollided();
	b2Body* getCollidingBodyA();
	b2Body* getCollidingBodyB();
	b2Body* getPlayerBody();
	int getLives();
	bool isOnGround();
	bool getIsFacingLeft();
	bool getIsFacingRight();
	int getRubiesCollected();
	void setRubiesCollected(int amount);
	
private:

	void switchButtonsDown(float dt);
	void checkSticks(float dt);
	void move(char direction, float scale, float dt);
	void resetPosition();
	void checkBounds();
	void buildTransform();
	void jump(float dt);

	// MODEL STUFF
	void loadAsset(gef::Platform& platform, const char* assetFilePath);
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);
	gef::Skeleton* GetFirstSkeleton(gef::Scene* scene);
	gef::Animation* LoadAnimation(const char* anim_scene_filename, const char* anim_name);

	// MODEL / MESH STUFF
	gef::Scene* playerModel;
	gef::SkinnedMeshInstance* playerSkinnedMesh;
	MotionClipPlayer animationPlayer;
	float meshXdimension;
	float meshYDimension;

	// ANIMATION STUFF
	gef::Animation* idleAnimation;
	gef::Animation* runAnimation;
	gef::Animation* jumpAnimation;

	// PLATFORM / CONTROLLER STUFF
	gef::Platform* m_platform;
	gef::InputManager* m_im;
	gef::SonyControllerInputManager* m_scim;
	const gef::SonyController* m_controller;

	bool xPressed;
	bool squarePressed;
	bool circlePressed;
	bool trianglePressed;
	bool L1Pressed;
	bool L2Pressed;
	bool L3Pressed;
	bool R1Pressed;
	bool R2Pressed;
	bool R3Pressed;
	bool upDPressed;
	bool downDPressed;
	bool leftDPressed;
	bool rightDPressed;
	bool sharePressed;
	bool optionsPressed;
	bool quit;

	float leftStickX;
	float rightStickX;
	float leftStickY;
	float rightStickY;
	float rightStickAngleRad;
	float rightStickAngleDeg;
	float leftStickAngleRad;
	float leftStickAngleDeg;

	// PLAYER STUFF
	b2BodyDef playerBodyDef;
	b2Body* playerBody;
	b2PolygonShape playerPolygonShape;
	b2FixtureDef playerFixtureDef;
	float playerYPos;
	float speed;
	float maxSpeed;
	float jumpForce;
	bool onGround;
	int lives;
	bool facingLeft;
	bool facingRight;
	bool isJumping;
	bool isMoving;
	int rubiesCollected;
	bool hasCollided;
	bool collidedWithEnemy;

	// COLLISION STUFF
	b2Body* bodyA;
	b2Body* bodyB;
	float collisionTimer;
	float recoveryTimer;
	bool canCollideAgain;

	// AUDIO STUFF
	gef::AudioManager* audioManager;
	Int32 collectItemSFX;
	Int32 jumpSFX;
	Int32 hurtSFX;
	gef::VolumeInfo* playerSFXVolinfo;

	float stateTimer;
};

/////////////////////////////////////////////////////////////////////////////////////////////////