#pragma once
#include <graphics/renderer_3d.h>
#include <graphics/mesh_instance.h>
#include <maths/quaternion.h>
#include <input/sony_controller_input_manager.h>
#include <input/input_manager.h>
#include <cmath>
#include <maths/math_utils.h>

#include "Player.h"

class Enemy : public GameObject
{
public:
	Enemy(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation);
	~Enemy();

	void handleInput(float dt);
	void update(float dt);
	void render(gef::Renderer3D* rend3D);

	void move(char direction, float scale, float dt);
	void checkBounds();
	void followPlayer(Player* player, float dt);
	void buildTransform();

	gef::Vector4* getPosition();
	void setPosition(gef::Vector4* newPosition);
	void setRotation(gef::Vector4 newRot);

private:

	float speed = 0.5f;

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