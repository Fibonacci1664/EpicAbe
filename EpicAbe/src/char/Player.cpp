/*
 * This class controls all aspects of the player including:
 *		- Initilaising Physics body.
 *		- Initialising the model and animations.
 *		- Handling input.
 *		- Item Collection.
 *		- Health.
 *		- Collisions with environment, Collectable, or Enemy.
 *  	- Rendering the mesh.
 *		- Updating the transform.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2021.
 */

/////////////////////////////////////////////////////////////////////////////////////////////////

#include "Player.h"
#include "../collectable/Ruby.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/scene.h>
#include <animation/skeleton.h>
#include <graphics/skinned_mesh_instance.h>
#include <animation/animation.h>
#include <graphics/mesh.h>
#include <audio/audio_manager.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
Player::Player(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation) :
	xPressed(false),
	squarePressed(false),
	circlePressed(false),
	trianglePressed(false),
	L1Pressed(false),
	L2Pressed(false),
	L3Pressed(false),
	R1Pressed(false),
	R2Pressed(false),
	R3Pressed(false),
	upDPressed(false),
	downDPressed(false),
	leftDPressed(false),
	rightDPressed(false),
	sharePressed(false),
	optionsPressed(false),
	quit(false)
{
	// COLLISION STUFF
	bodyA = nullptr;
	bodyB = nullptr;
	collisionTimer = 0;
	recoveryTimer = 0;
	canCollideAgain = true;

	// ANIMATION STUFF
	idleAnimation = nullptr;
	runAnimation = nullptr;
	jumpAnimation = nullptr;

	// PHYSICS BODY STUFF
	playerBody = nullptr;

	// MODEL / MESH STUFF
	playerModel = nullptr;
	playerSkinnedMesh = nullptr;
	meshXdimension = 0;
	meshYDimension = 0;

	// PLATFORM / CONTROLLER STUFF
	m_platform = nullptr;
	m_im = nullptr;
	m_scim = nullptr;
	m_controller = nullptr;
	leftStickX = 0;
	rightStickX = 0;
	leftStickY = 0;
	rightStickY = 0;
	rightStickAngleRad = 0;
	rightStickAngleDeg = 0;
	leftStickAngleRad = 0;
	leftStickAngleDeg = 0;

	// PLAYER STUFF
	m_startPos = position;			// Start position is saved in case we ever need to reset back to start pos.
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;
	playerYPos = 0;
	speed = 50;
	maxSpeed = 3;
	jumpForce = 60.0f;		// THIS NEEDS TO BE SET V LOW IF USING IMPULSE.
	lives = 3;
	onGround = false;
	hasCollided = false;
	collidedWithEnemy = false;
	facingLeft = false;
	facingRight = true;
	isJumping = false;
	isMoving = false;
	rubiesCollected = 0;	

	// AUDIO STUFF
	audioManager = nullptr;
	playerSFXVolinfo = nullptr;
	collectItemSFX = 0;
	jumpSFX = 0;
	hurtSFX = 0;

	stateTimer = 0;
}

Player::~Player()
{
	delete playerModel;
	playerModel = nullptr;

	delete playerSkinnedMesh;
	playerSkinnedMesh = nullptr;

	delete idleAnimation;
	idleAnimation = nullptr;

	delete runAnimation;
	runAnimation = nullptr;

	delete jumpAnimation;
	jumpAnimation = nullptr;

	delete playerSFXVolinfo;
	playerSFXVolinfo = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void Player::handleInput(float dt)
{
	// HANDLE INPUT
	setButtonState();
	checkSticks(dt);
	switchButtonsDown(dt);
	// END HANDLE INPUT
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool Player::update(float dt, b2World* world_)
{
	stateTimer += dt;

	if (playerSkinnedMesh)
	{
		// update the pose in the anim player from the animation
		animationPlayer.Update(dt, playerSkinnedMesh->bind_pose());

		// update the bone matrices that are used for rendering the character
		// from the newly updated pose in the anim player
		playerSkinnedMesh->UpdateBoneMatrices(animationPlayer.pose());
	}

	if (isJumping)
	{
		animationPlayer.set_looping(true);
		animationPlayer.set_playback_speed(0.75);
	}

	if (collisionTimer > 2.0f || stateTimer > 2.0f)
	{
		canCollideAgain = true;
		collisionTimer = 0;
		stateTimer = 0;
	}

	if (collidedWithEnemy)
	{
		audioManager->PlaySample(hurtSFX, false);
		--lives;
		collidedWithEnemy = false;
		canCollideAgain = false;
	}

	checkCollisions(dt, world_);
	buildTransform();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::render(gef::Renderer3D* rend3D)
{
	// draw the player, the pose is defined by the bone matrices
	if (playerSkinnedMesh)
	{
		rend3D->DrawSkinnedMesh(*this, playerSkinnedMesh->bone_matrices());
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::initInputManager(gef::Platform& platform,
	gef::InputManager* im,
	gef::SonyControllerInputManager* scim,
	const gef::SonyController* controller)
{
	m_platform = &platform;

	m_im = im;
	m_scim = scim;
	m_controller = controller;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::checkCollisions(float dt, b2World* world_)
{
	// collision detection
	// get the head of the contact list
	b2Contact* contact = world_->GetContactList();
	// get contact count
	int contact_count = world_->GetContactCount();

	for (int contact_num = 0; contact_num < contact_count; ++contact_num)
	{
		if (contact->IsTouching())
		{
			// get the colliding bodies
			bodyA = contact->GetFixtureA()->GetBody();
			bodyB = contact->GetFixtureB()->GetBody();

			//gef::DebugOut("A collision has occured!\n");
			hasCollided = true;

			/* Cast the b2Body* to a GameObject*, Setting the GameObject pointer on objectA
			 * to point to the same location as the user data on bodyA.
			 */
			GameObject* objectA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
			GameObject* objectB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);

			// Make sure both are not null.
			if (objectA && objectB)
			{
				// Check for collisions with enemy
				if (*objectA->getType() == ObjectType::ENEMY && *objectB->getType() == ObjectType::PLAYER
					|| *objectB->getType() == ObjectType::ENEMY && *objectA->getType() == ObjectType::PLAYER)
				{
					collisionTimer += dt;

					if (canCollideAgain)
					{
						collidedWithEnemy = true;
					}					
				}
				
				// Check for collisions with ground
				if (*objectA->getType() == ObjectType::ENVIRONMENT && *objectB->getType() == ObjectType::PLAYER
					|| *objectB->getType() == ObjectType::ENVIRONMENT && *objectA->getType() == ObjectType::PLAYER)
				{
					onGround = true;
					isJumping = false;
				}

				// Set for collisions with collectable
				if (*objectA->getType() == ObjectType::COLLECTABLE && *objectB->getType() == ObjectType::PLAYER
					|| *objectB->getType() == ObjectType::COLLECTABLE && *objectA->getType() == ObjectType::PLAYER)
				{
					if (*objectB->getType() == ObjectType::COLLECTABLE)
					{
						// play sfx
						audioManager->PlaySample(collectItemSFX, false);

						// This is fine, this gets me access to the same ruby object.
						Ruby* ruby = reinterpret_cast<Ruby*>(bodyB->GetUserData().pointer);
						ruby->setIsAlive(false);
						++rubiesCollected;
					}
				}
			}
		}
		else
		{
			hasCollided = false;
		}

		// Get next contact point
		contact = contact->GetNext();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::setButtonState()
{
	xPressed = false;
	squarePressed = false;
	circlePressed = false;
	trianglePressed = false;
	L1Pressed = false;
	L2Pressed = false;
	L3Pressed = false;
	R1Pressed = false;
	R2Pressed = false;
	R3Pressed = false;
	upDPressed = false;
	downDPressed = false;
	leftDPressed = false;
	rightDPressed = false;
	sharePressed = false;
	optionsPressed = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::switchButtonsDown(float dt)
{
	if (m_scim != nullptr)
	{
		if (m_controller != nullptr)
		{
			switch (m_controller->buttons_down())
			{
			case gef_SONY_CTRL_CROSS:
				xPressed = true;
				if (onGround)
				{
					jump(dt);
				}
				break;
			case gef_SONY_CTRL_SQUARE:
				squarePressed = true;
				break;
			case gef_SONY_CTRL_CIRCLE:
				circlePressed = true;
				break;
			case gef_SONY_CTRL_TRIANGLE:
				trianglePressed = true;
				resetPosition();
				break;
			case gef_SONY_CTRL_L1:
				L1Pressed = true;
				break;
			case gef_SONY_CTRL_L2:
				L2Pressed = true;
				break;
			case gef_SONY_CTRL_L3:
				L3Pressed = true;
				break;
			case gef_SONY_CTRL_R1:
				R1Pressed = true;
				break;
			case gef_SONY_CTRL_R2:
				R2Pressed = true;
				break;
			case gef_SONY_CTRL_R3:
				R3Pressed = true;
				break;
			case gef_SONY_CTRL_UP:
				upDPressed = true;
				break;
			case gef_SONY_CTRL_DOWN:
				downDPressed = true;
				break;
			case gef_SONY_CTRL_LEFT:
				leftDPressed = true;
				move('l', -1, dt);	// For movement with D-Pad instead of sticks.
				break;
			case gef_SONY_CTRL_RIGHT:
				rightDPressed = true;
				move('r', 1, dt);		// For movement with D-Pad instead of sticks.
				break;
			case gef_SONY_CTRL_START:
				sharePressed = true;
				break;
			case gef_SONY_CTRL_SELECT:
				optionsPressed = true;
				break;
			default:
				break;
			}

			if ((m_controller->buttons_down() & gef_SONY_CTRL_START) && (m_controller->buttons_down() & gef_SONY_CTRL_SELECT))
			{
				// Quit application.
				quit = true;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::checkSticks(float dt)
{
	if (m_scim != nullptr)
	{
		if (m_controller != nullptr)
		{
			leftStickX = m_controller->left_stick_x_axis();
			leftStickY = m_controller->left_stick_y_axis();

			// Ensure we dont get any divide by zero errors.
			if (leftStickX == 0)
			{
				leftStickX = LDBL_EPSILON;
			}

			leftStickAngleRad = std::atan(leftStickY / leftStickX);
			leftStickAngleDeg = gef::RadToDeg(leftStickAngleRad);

			// Get the left stick axis value again, as we WANT it to be zero now
			// based on conditionals from here on, there is no division by leftStickX
			// So no worries of divide by zero after this point.
			leftStickX = m_controller->left_stick_x_axis();

			// If we're pushing right on left stick.
			if (leftStickX >= 0)
			{
				leftStickAngleDeg += 90;
			}
			else if (leftStickX <= 0)	// If we're pushing left on left stick.
			{
				leftStickAngleDeg += 270;
			}

			// Logic for setting rotation of player.
			if ((leftStickX < 0.001 && leftStickX > -0.001) && (leftStickY < 0.001 && leftStickY > -0.001))
			{
				//rotating = false;
				leftStickAngleRad = 0;
				leftStickAngleDeg = 0;
			}

			// Control movement with left thumb stick
			if (leftStickX < 0)
			{
				move('l', leftStickX, dt);
			}
			else if (leftStickX > 0)
			{
				move('r', leftStickX, dt);
			}
			else
			{
				if (!isJumping)
				{
					animationPlayer.set_clip(idleAnimation);
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::move(char direction, float scale, float dt)
{
	// UP = -1 and DOWN = 1.
	switch (direction)
	{
	case 'r':
		//m_position += gef::Vector4(scale, 0, 0) * dt * speed;
		if (playerBody->GetLinearVelocity().x < maxSpeed)
		{
			setRotation(gef::Vector4(0, 3.1415 * 0.5f, 0));
			playerBody->ApplyForceToCenter(b2Vec2(scale * speed, 0), true);
			facingRight = true;
			facingLeft = false;
		}
		break;
	case 'l':
		//m_position += gef::Vector4(scale, 0, 0) * dt * speed;
		if (playerBody->GetLinearVelocity().x > -maxSpeed)
		{
			setRotation(gef::Vector4(0, 3.1415 * 1.5f, 0));
			playerBody->ApplyForceToCenter(b2Vec2(scale * speed, 0), true);
			facingLeft = true;
			facingRight = false;
		}
		break;
	default:
		break;
	}

	if (onGround)
	{
		animationPlayer.set_clip(runAnimation);
		animationPlayer.set_looping(true);
		animationPlayer.set_playback_speed(std::abs(scale));
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::resetPosition()
{
	m_position = m_startPos;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::checkBounds()
{
	// Need to setup AABB for this! I think meshes already have AABB setup when created.
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::buildTransform()
{
	UpdateFromSimulation(playerBody);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::jump(float dt)
{
	isJumping = true;
	onGround = false;

	audioManager->PlaySample(jumpSFX, false);

	animationPlayer.set_clip(jumpAnimation);
	animationPlayer.set_anim_time(animationPlayer.clip()->start_time());

	// If we pressed the jump button, we must be in the air, i.e. we're NOT on the ground.
	playerBody->ApplyForceToCenter(b2Vec2(0, jumpForce), true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::initModelPlayer()
{
	currentType = ObjectType::PLAYER;
	playerModel = new gef::Scene();
	loadAsset(*m_platform, "abe.scn");
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::initPhysicsBody(b2World* world)
{
	// Set up box2d dynamic body for player.
	playerBodyDef.type = b2_dynamicBody;
	playerBodyDef.position.Set(m_position.x(), m_position.y());
	playerBodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
	playerBodyDef.fixedRotation = true;

	// Create and attach the player body def.
	playerBody = world->CreateBody(&playerBodyDef);
	
	// Set up the players invisible physics body shape, this needs to match the mesh sizes obv or you'll get strange collision behaviour.
	playerPolygonShape.SetAsBox(meshXdimension * 0.001f, meshYDimension * 0.00005f);

	// Set up the players body fixture.
	playerFixtureDef.shape = &playerPolygonShape;
	playerFixtureDef.density = 80.0f;
	playerFixtureDef.friction = 1.0f;

	// Create and attach the players fixture.
	playerBody->CreateFixture(&playerFixtureDef);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::initAudio(gef::AudioManager* audioMan, gef::Platform& thePlatform)
{
	audioManager = audioMan;

	collectItemSFX = audioManager->LoadSample("../media/itemCollect.wav", thePlatform);
	jumpSFX = audioManager->LoadSample("../media/jump.wav", thePlatform);
	hurtSFX = audioManager->LoadSample("../media/hurt.wav", thePlatform);

	playerSFXVolinfo = new gef::VolumeInfo();
	playerSFXVolinfo->volume = 75;
	audioManager->SetMusicVolumeInfo(*playerSFXVolinfo);
}

/////////////////////////////////////////////////////////////////////////////////////////////////


void Player::loadAsset(gef::Platform& platform, const char* assetFilePath)
{
	const char* asset = assetFilePath;
	playerModel = LoadSceneAssets(platform, assetFilePath);
	mesh_ = GetMeshFromSceneAssets(playerModel);

	meshXdimension = mesh_->aabb().max_vtx().x() - mesh_->aabb().min_vtx().x();		// Rtns size of mesh in x.
	meshYDimension = mesh_->aabb().max_vtx().y() - mesh_->aabb().min_vtx().y();		// Rtns size of mesh in y.

	gef::Skeleton* playerSkeleton = GetFirstSkeleton(playerModel);

	if (playerSkeleton)
	{
		playerSkinnedMesh = new gef::SkinnedMeshInstance(*playerSkeleton);
		animationPlayer.Init(playerSkinnedMesh->bind_pose());
		playerSkinnedMesh->set_mesh(mesh_);
	}

	// anims
	idleAnimation = LoadAnimation("@idle.scn", "");
	runAnimation = LoadAnimation("@run.scn", "");
	jumpAnimation = LoadAnimation("@jump.scn", "");

	if (idleAnimation)
	{
		animationPlayer.set_clip(idleAnimation);
		animationPlayer.set_looping(true);
		animationPlayer.set_anim_time(0.0f);
		animationPlayer.set_playback_speed(1.0f);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Animation* Player::LoadAnimation(const char* anim_scene_filename, const char* anim_name)
{
	gef::Animation* anim = NULL;

	gef::Scene anim_scene;

	if (anim_scene.ReadSceneFromFile(*m_platform, anim_scene_filename))
	{
		// if the animation name is specified then try and find the named anim
		// otherwise return the first animation if there is one
		std::map<gef::StringId, gef::Animation*>::const_iterator anim_node_iter;

		if (anim_name)
			anim_node_iter = anim_scene.animations.find(gef::GetStringId(anim_name));
		else
			anim_node_iter = anim_scene.animations.begin();

		if (anim_node_iter != anim_scene.animations.end())
			anim = new gef::Animation(*anim_node_iter->second);
	}

	return anim;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Scene* Player::LoadSceneAssets(gef::Platform& platform, const char* filename)
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

gef::Mesh* Player::GetMeshFromSceneAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	// if the scene data contains at least one mesh
	// return the first mesh
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();

	return mesh;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

gef::Skeleton* Player::GetFirstSkeleton(gef::Scene* scene)
{
	gef::Skeleton* skeleton = NULL;
	if (scene)
	{
		// check to see if there is a skeleton in the the scene file
		// if so, pull out the bind pose and create an array of matrices
		// that wil be used to store the bone transformations
		if (scene->skeletons.size() > 0)
			skeleton = scene->skeletons.front();
	}

	return skeleton;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// GETTERS / SETTERS
gef::Vector4* Player::getPosition()
{
	return &m_position;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::setRotation(gef::Vector4 newRot)
{
	m_rotation.set_x(newRot.x());
	m_rotation.set_y(newRot.y());
	m_rotation.set_z(newRot.z());
}

/////////////////////////////////////////////////////////////////////////////////////////////////

float Player::getLeftStickAngle()
{
	return leftStickAngleDeg;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

float Player::getPlayerYPos()
{
	return playerYPos;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool Player::getHasCollided()
{
	return hasCollided;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

b2Body* Player::getCollidingBodyA()
{
	return bodyA;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

b2Body* Player::getCollidingBodyB()
{
	return bodyB;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

b2Body* Player::getPlayerBody()
{
	return playerBody;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

int Player::getLives()
{
	return lives;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool Player::isOnGround()
{
	return onGround;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool Player::getIsFacingLeft()
{
	return facingLeft;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool Player::getIsFacingRight()
{
	return facingRight;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

int Player::getRubiesCollected()
{
	return rubiesCollected;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::setRubiesCollected(int amount)
{
	rubiesCollected += amount;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

const gef::SonyController* Player::getSonyController()
{
	return m_controller;
}

/////////////////////////////////////////////////////////////////////////////////////////////////