/*
 * This class controls
 *		- Initialising a physics body for in game collectable hearts.
 *		- Updating heart objects based on whether they are in game collectable
 *	      or HUD hearts.
 *		- Destroying itself and the physics body once collected.
 *		- Logic for controlling the animation of hearts.
 *			- In game collectable hearts bounce.
 *			- HUD hearts rotate.
 *		- * NOTE *
 *		- IN GAME COLLECTABLE HEARTS NOT YET IMPLEMENTED BUT ALL THE ABOVE STILL
 *		- EXISTS FOR POSSIBLE FUTURE DEVELOPMENT.
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

class Heart : public GameObject
{
public:
	Heart(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation, bool isHUD);
	~Heart();

	void update(float dt);
	void render(gef::Renderer3D* rend3D);
	void initHeart(b2World* world, float xMeshSize, float yMeshSize);
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
	bool isHUDHeart;
	float force;
	bool alive;
	bool bodyDestroyed;

	// ENV PHYSICS BODY STUFF
	b2BodyDef heartBodyDef;
	b2Body* heartBody;
	b2PolygonShape heartPolygonShape;
	b2FixtureDef heartFixDef;
	b2World* theWorld;
};

/////////////////////////////////////////////////////////////////////////////////////////////////