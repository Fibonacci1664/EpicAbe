/*
 * This class controls
 *		- Initialising a physics body for in game collectable rubies.
 *		- Updating ruby objects based on whether they are in game collectable
 *	      or HUD rubies.
 *		- Destroying itself and the physics body once collected.
 *		- Logic for controlling the animation of rubies.
 *			- In game collectable rubies bounce.
 *			- HUD rubies rotate.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2021.
 */

/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../object/game_object.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/renderer_3d.h>
#include <box2d/box2d.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

class Ruby : public GameObject
{
public:
	Ruby(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation, bool isHUD);
	~Ruby();

	void update(float dt);
	void render(gef::Renderer3D* rend3D);
	void initRuby(b2World* world, float xMeshSize, float yMeshSize);
	void buildTransform();
	void move(float dt);
	void destroyBody();

	void setIsAlive(bool isAlive);
	bool getIsAlive();
	bool getIsBodyDestroyed();

	void setPosition(gef::Vector4 position);
	void setRotation(gef::Vector4 newRot);

private:

	float speed;
	bool isHUDRuby;
	float force;
	bool alive;
	bool bodyDestroyed;

	// ENV PHYSICS BODY STUFF
	b2BodyDef rubyBodyDef;
	b2Body* rubyBody;
	b2PolygonShape rubyPolygonShape;
	b2FixtureDef rubyFixDef;
	b2World* theWorld;
};

/////////////////////////////////////////////////////////////////////////////////////////////////