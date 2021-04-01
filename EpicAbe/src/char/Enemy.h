#pragma once
#include "../object/game_object.h"

#include <graphics/renderer_3d.h>
#include <graphics/mesh_instance.h>
#include <input/sony_controller_input_manager.h>
#include <input/input_manager.h>
#include <cmath>
#include <maths/math_utils.h>

// FORWARD DECS
class Player;

class Enemy : public GameObject
{
public:
	Enemy(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation);
	~Enemy();

	void handleInput(float dt);
	void update(float dt);
	void render(gef::Renderer3D* rend3D);

	void initEnemy(b2World* world, float xMeshSize, float yMeshSize);
	void move(float dt);
	void followPlayer(Player* player, float dt);
	void buildTransform();

	gef::Vector4* getPosition();
	void setPosition(gef::Vector4* newPosition);
	void setRotation(gef::Vector4 newRot);
	void setSpeed(float newSpeed);
	void setStartLocation(float startLoc);

private:

	float speed;

	gef::Vector4 m_scale;
	gef::Vector4 m_rotation;
	gef::Vector4 m_position;

	gef::Vector4 m_startPos;

	// ENEMY PHYSICS BODY STUFF
	b2BodyDef enemyBodyDef;
	b2Body* enemyBody;
	b2PolygonShape enemyPolygonShape;
	b2FixtureDef enemyFixtureDef;
};