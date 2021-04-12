/*
 * This class controls:
 *		- Rendering the mesh of the model.
 *		- Initialising the physics body of the object.
 *		- Building the transform of the object.
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

class EnvPlatform : public GameObject
{
public:
	EnvPlatform(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation);
	~EnvPlatform();

	void update(float dt);
	void render(gef::Renderer3D* rend3D);
	void initEnvPlatform(b2World* world, float xMeshSize, float yMeshSize);
	void buildTransform();

private:

	// ENV PHYSICS BODY STUFF
	b2BodyDef envBodyDef;
	b2Body* envBody;
	b2PolygonShape envPolygonShape;
	b2FixtureDef envFixDef;
};

/////////////////////////////////////////////////////////////////////////////////////////////////